#include "Speculator.hh"

void Speculator::Halt(string err)
{
    cerr << "Speculator halted: " << err << endl;
    exit(-1);
}

void Speculator::setReg(uint8_t num, uint32_t val){
    if(num == 0 || num >= REG_NUM)
    {
        Halt("Cannot change register.");
    }
    R[num] = val;
}

uint32_t Speculator::readReg(uint8_t num){
    if(num >= REG_NUM)
        Halt("No such register.");
    return R[num];
}

void Speculator::load(ifstream &f, bool priv){
    uint32_t TextSize = 0;
    uint32_t DataSize = 0;
    uint32_t reader = 0;

    uint32_t codeSection = priv ? (CODE_START | 0x80000000) : CODE_START;
    uint32_t dataSection = priv ? (DATA_START | 0x80000000) : DATA_START;

    TextSize = Next4Bytes(f);
    DataSize = Next4Bytes(f);
    IMSIZE = TextSize;


    for(uint32_t i = 0; i < TextSize + 1; i += PAGE_SIZE)
        Memory.AllocMem(codeSection + i, priv, PERM_EXEC | PERM_READ);

    for(uint32_t i = 0; i < DataSize + 1; i += PAGE_SIZE)
        Memory.AllocMem(dataSection + i, priv, PERM_READ | PERM_WRITE);

    for(uint32_t i = 0; i < TextSize; i += 4){
        reader = Next4Bytes(f);
        Memory.WriteMem(codeSection + i, reader);

    }

    for(uint32_t i = 0; i < DataSize; i += 4){
        reader = Next4Bytes(f);
        Memory.WriteMem(dataSection + i, reader);
    }
    f.close();
}

void Speculator::run (void) {
    struct _pc     PC_buffer;
    struct _IF_ID  IF_ID_buffer;
    struct _ID_EX  ID_EX_buffer;
    struct _EX_MEM EX_MEM_buffer;
    struct _MEM_WB MEM_WB_buffer;


    while (true) {
        cycle++;
        PC_buffer     = (const struct _pc) { 0 };
        PC_buffer.valid = PC.valid;
        PC_buffer.pc = PC.pc;
        IF_ID_buffer  = (const struct _IF_ID){ 0 };
        ID_EX_buffer  = (const struct _ID_EX){ 0 };
        EX_MEM_buffer = (const struct _EX_MEM){ 0 };
        MEM_WB_buffer = (const struct _MEM_WB){ 0 };

        WriteBack();
        Dec(&PC_buffer, &ID_EX_buffer);
        Execute(&EX_MEM_buffer);
        MemAccess(&PC_buffer, &MEM_WB_buffer);
        SelectPC(&PC_buffer);
        IFetch(&IF_ID_buffer);

        Update(PC_buffer, IF_ID_buffer, ID_EX_buffer, EX_MEM_buffer, MEM_WB_buffer);

        if(!PC.valid && !IF_ID.valid && !ID_EX.valid && !EX_MEM.valid && !MEM_WB.valid)
            return;
    }
}

void Speculator::SelectPC(struct _pc *pc_buffer){
    if(!(PC.valid | pc_buffer->valid)) {
        return;
    }

    pc_buffer->valid = true;
    switch(pc_buffer->pPort){
        case EXCEPTION:
            pc_buffer->pc = pc_buffer->jtarget;
            FLUSH |= (L_IF_ID);
            break;
        case JUMP:
            PC.pc = pc_buffer->jtarget;
        default:
            pc_buffer->pc = PC.pc + 4;
            break;
    }
}

void Speculator::IFetch(struct _IF_ID *L){
   if(!PC.valid){
        *L = (const struct _IF_ID) { 0 };
        return;
    }

    L->valid = true;
    L->pc = PC.pc;
    L->NPC = PC.pc + 4;
    L->INT_CAUSE = 0;
    L->EPC_WRITE = false;
    L->inst = Memory.ReadMem(PC.pc, true, priv, &L->INT_CAUSE, &L->EPC_WRITE);
}

void Speculator::Dec(struct _pc *PL, struct _ID_EX *L){
    if(!IF_ID.valid){
        *L = (const struct _ID_EX) { 0 };
        return;
    }
    if(IF_ID.EPC_WRITE)
    {
        *L = (const struct _ID_EX) { 0 };
        L->INT_CAUSE = ID_EX.INT_CAUSE;
        L->EPC_WRITE = ID_EX.EPC_WRITE;
        return;
    }
    L->valid = true;
    L->pc = IF_ID.pc;
    L->NPC = IF_ID.NPC;
    union Format Decoder;
    Decoder.raw = IF_ID.inst;

     if(ID_EX.MemRead
        && ((ID_EX.RegDst == Decoder.R.rs) || (ID_EX.RegDst == Decoder.R.rt)))
     {
         Hazard.ex_stall = 1;
     }
     if(Hazard.ex_stall > 0)
         return;

    L->reg1 = readReg(Decoder.R.rs);
    L->reg1num = Decoder.R.rs;
    L->reg2 = readReg(Decoder.R.rt);
    L->reg2num = Decoder.R.rt;
    L->imm = (int32_t)((int16_t)Decoder.I.imm);
    L->shamt = Decoder.R.shamt;
    L->ALUSrc = aIMM;
    L->MemRead = false;
    L->MemWrite = false;
    L->EPC_WRITE = IF_ID.EPC_WRITE;
    L->INT_CAUSE = IF_ID.INT_CAUSE;
    L->RegWrite = true;
    L->MemToReg = ALU_result;
    if(L->EPC_WRITE == true)
    {
        L->ALUOp = NAN;
        return;
    }

    switch(Decoder.R.op){
        case RTYPE:
            L->ALUSrc = aREG;
            L->RegDst = Decoder.R.rd;
            switch(Decoder.R.funct){
                case _ADDU:
                    L->ALUOp = ADD;
                    break;
                case _AND:
                    L->ALUOp = AND;
                    break;
                case _OR:
                    L->ALUOp = OR;
                    break;
                case _SLTU:
                    L->ALUOp = SLT;
                    break;
                case _SLL:
                    L->ALUOp = SLL;
                    break;
                case _SRL:
                    L->ALUOp = SRL;
                    break;
                case _SUBU:
                    L->ALUOp = SUB;
                    break;
                case _XOR:
                    L->ALUOp = XOR;
                    break;
                case _JR:
                    PL->valid = true;
                    PL->jtarget = readReg(Decoder.R.rs);
                    PL->pPort = JUMP;
                    L->RegWrite = false;
                    L->ALUOp = NAN;
                    break;
                case _SYSCALL:
                    L->ALUOp = NAN;
                    L->INT_CAUSE = INT_SYSCALL;
                    L->EPC_WRITE = true;
                    break;
                case _EXIT:
                    L->ALUOp = EXIT;
                    break;
                default:
                    L->ALUOp = NAN;
                    L->INT_CAUSE = INT_INSERR;
                    L->EPC_WRITE = true;
                    break;
            }
            break;
        case _ADDIU:
            L->ALUOp = ADD;
            L->RegDst = Decoder.I.rt;
            break;
        case _ANDI:
            L->ALUOp = AND;
            L->RegDst = Decoder.I.rt;
            L->imm = Decoder.I.imm & 0xffff;
            break;
        case _BEQ:
            L->ALUOp = EQ;
            L->ALUSrc = aREG;
#ifdef BTB
            PL->valid = true;
            PL->jtarget = BTBM.predict(IF_ID.pc);
            PL->pPort = JUMP;
            L->Prediction = PL->jtarget;
#else
            PL->valid = true;
            PL->jtarget = L->NPC + (L->imm << 2);
            PL->pPort = JUMP;
#endif
            L->saved_pc = IF_ID.NPC;
            L->RegWrite = false;
            break;
        case _BNE:
            L->ALUOp = NEQ;
            L->ALUSrc = aREG;
#ifdef BTB
            PL->valid = true;
            PL->jtarget = BTBM.predict(IF_ID.pc);
            PL->pPort = JUMP;
            L->Prediction = PL->jtarget;
#else
            PL->valid = true;
            PL->jtarget = L->NPC + (L->imm << 2);
            PL->pPort = JUMP;
#endif
            L->saved_pc = IF_ID.NPC;
            L->RegWrite = false;
            break;
        case _LUI:
            L->ALUOp = LUI;
            L->RegDst = Decoder.I.rt;
            break;
        case _LW:
            L->ALUOp = ADD;
            L->RegDst = Decoder.I.rt;
            L->MemRead = true;
            L->MemToReg = Read_Data;
            break;
        case _ORI:
            L->ALUOp = OR;
            L->RegDst = Decoder.I.rt;
            L->imm = Decoder.I.imm & 0xffff;
            break;
        case _SLTIU:
            L->ALUOp = SLT;
            L->RegDst = Decoder.I.rt;
            break;
        case _SW:
            L->ALUOp = ADD;
            L->MemWrite = true;
            L->RegWrite = false;
            break;
        case _JAL:
            PL->valid = 1;
            PL->jtarget = Decoder.J.jump_target << 2;
            PL->pPort = JUMP;
            L->ALUOp = ADD;
            L->reg1 = 0;
            L->imm = L->NPC;
            L->RegDst = 63;
            break;
        case _J:
            PL->valid = 1;
            PL->jtarget = Decoder.J.jump_target << 2;
            PL->pPort = JUMP;
            L->RegWrite = false;
            L->ALUOp = NAN;
            break;
        case _RFE:
            L->ALUOp = NAN;
            L->RegWrite = false;
            if(priv)
            {
                FLUSH |= (L_PC | L_IF_ID);
                L->INT_CAUSE = INT_RFE;
                L->EPC_WRITE = true;
            }
            else
            {
                L->INT_CAUSE = INT_INSERR;
                L->EPC_WRITE = true;
                break;
            }
            break;
        default:
            L->ALUOp = NAN;
            L->RegWrite = false;
            L->INT_CAUSE = INT_INSERR;
            L->EPC_WRITE = true;
            break;
    }
    return;
}

void Speculator::Execute(struct _EX_MEM *L){
    if(!ID_EX.valid){
        *L = (const struct _EX_MEM){ 0 };
        return;
    }
    if(ID_EX.EPC_WRITE)
    {
        *L = (const struct _EX_MEM){ 0 };
        L->valid = true;
        L->pc = ID_EX.pc;
        L->INT_CAUSE = ID_EX.INT_CAUSE;
        L->EPC_WRITE = ID_EX.EPC_WRITE;
        return;
    }
    Forward.MUX1 = Forward.MUX2 = P01;
    if(EX_MEM.RegWrite &&
       (EX_MEM.RegDst != 0) &&
       (EX_MEM.RegDst == ID_EX.reg1num))
        Forward.MUX1 = P02;
    else if(MEM_WB.RegWrite &&
       (MEM_WB.RegDst != 0) &&
       (MEM_WB.RegDst == ID_EX.reg1num))
        Forward.MUX1 = P03;

    if(EX_MEM.RegWrite &&
       (EX_MEM.RegDst != 0) &&
       (EX_MEM.RegDst == ID_EX.reg2num))
        Forward.MUX2 = P02;
    else if(MEM_WB.RegWrite &&
       (MEM_WB.RegDst != 0) &&
       (MEM_WB.RegDst == ID_EX.reg2num))
        Forward.MUX2 = P03;


    uint32_t op1, op2, result;
    switch(Forward.MUX1){
        case P01:
            op1 = ID_EX.reg1;
            break;
        case P02:
            op1 = EX_MEM.ALU_OUT;
            break;
        case P03:
            op1 = MEM_WB.MemToReg ? MEM_WB.MEM_OUT : MEM_WB.ALU_OUT;
            break;
    }

    switch(Forward.MUX2){
        case P01:
            op2 = ID_EX.reg2;
            break;
        case P02:
            op2 = EX_MEM.ALU_OUT;
            break;
        case P03:
            op2 = MEM_WB.MemToReg ? MEM_WB.MEM_OUT : MEM_WB.ALU_OUT;
            break;
    }
    op2 = (ID_EX.ALUSrc == aREG) ? op2 : ID_EX.imm;

    L->valid = true;
    L->RegDst = ID_EX.RegDst;

    L->saved_pc = ID_EX.saved_pc;
    L->MemRead = ID_EX.MemRead;
    L->MemWrite = ID_EX.MemWrite;

    L->RegWrite = ID_EX.RegWrite;
    L->MemToReg = ID_EX.MemToReg;
    L->pc = ID_EX.pc;
#ifdef BTB
    L->Prediction = ID_EX.Prediction;
#endif
    L->INT_CAUSE = ID_EX.INT_CAUSE;
    L->EPC_WRITE = ID_EX.EPC_WRITE;
    L->exited = false;

    switch(ID_EX.ALUOp){
        case ADD:
            result = op1 + op2;
            break;
        case AND:
            result = op1 & op2;
            break;
        case OR:
            result = op1 | op2;
            break;
        case SLT:
            result = op1 < op2;
            break;
        case SLL:
            result = op2 << ID_EX.shamt;
            break;
        case SRL:
            result = op2 >> ID_EX.shamt;
            break;
        case SUB:
            result = op1 - op2;
            break;
        case XOR:
            result = op1 ^ op2;
            break;
        case EQ:
            L->branch = (op1 == op2);
            L->branchop = true;
            break;
        case NEQ:
            L->branch = (op1 != op2);
            L->branchop = true;
            break;
        case LUI:
            result = ID_EX.imm << 16;
            break;
        case EXIT:
            L->exited = true;
            break;
        case NAN:
            break;
    }

    L->ALU_OUT = result;
    L->BR_TARGET = ID_EX.NPC + (ID_EX.imm << 2);
    L->wdata = ID_EX.reg2;
#ifdef CACHE
    if(L->MemRead || L->MemWrite || L->MemToReg) {
#ifdef DEBUG
        printf("LoadCache %x in EX(%x) stage\n", L->ALU_OUT, L->pc);
#endif
        Memory.LoadCache(L->ALU_OUT, priv, &L->INT_CAUSE, &L->EPC_WRITE);
    }
#endif
}


void Speculator::MemAccess(struct _pc *pc_buf, struct _MEM_WB *L){
    if(!EX_MEM.valid){
        *L = (const struct _MEM_WB){ 0 };
        return;
    }
    uint32_t cause = EX_MEM.INT_CAUSE;
    bool epc_write = EX_MEM.EPC_WRITE;
    if(epc_write)
    {
        *L = (const struct _MEM_WB){ 0 };
        goto EPC;
    }

    if(EX_MEM.branchop){
#ifdef BTB
        uint32_t target = EX_MEM.branch ? EX_MEM.BR_TARGET : EX_MEM.saved_pc;
        if(EX_MEM.Prediction != target)
        {
            FLUSH |= (L_ID_EX | L_EX_MEM);
            pc_buf->valid = true;
            pc_buf->jtarget = target;
            pc_buf->pPort = JUMP;
        }
        BTBM.commitResult(EX_MEM.pc, target);
#else
        if(!EX_MEM.branch)
        {
            FLUSH |= (L_ID_EX | L_EX_MEM);
            pc_buf->valid = true;
            pc_buf->jtarget = EX_MEM.saved_pc;
            pc_buf->pPort = JUMP;
        }
#endif
    }


    if(EX_MEM.MemRead || EX_MEM.MemToReg) {
#ifndef CACHE
        L->MEM_OUT = Memory.ReadMem(EX_MEM.ALU_OUT, false, priv, &cause, &epc_write);
#else
        L->MEM_OUT = Memory.ReadCache(EX_MEM.ALU_OUT, &cause, &epc_write);
#endif
    }
    else if(EX_MEM.MemWrite) {
#ifndef CACHE
        Memory.WriteMem(EX_MEM.ALU_OUT, EX_MEM.wdata, priv, &cause, &epc_write);
#else
        Memory.WriteCache(EX_MEM.ALU_OUT, EX_MEM.wdata, &cause, &epc_write);
#endif
    }

    L->valid = true;
    L->pc = EX_MEM.pc;
    L->ALU_OUT = EX_MEM.ALU_OUT;
    L->RegDst = EX_MEM.RegDst;
    L->RegWrite = EX_MEM.RegWrite;
    L->MemToReg = EX_MEM.MemToReg;

EPC:
    if(epc_write)
    {
        switch(cause)
        {
            case INT_SYSCALL:
                L->valid = true;
                L->EPC_WRITE = true;
                L->epc = EX_MEM.pc + 4;
                L->priv = true;
                FLUSH |= (L_ID_EX | L_EX_MEM);
                pc_buf->valid = true;
                pc_buf->jtarget = CODE_START | 0x80000000;
                pc_buf->pPort = EXCEPTION;
                break;
            case INT_BUSERR:
                Halt("Speculator :: Bus Error.");
                break;
            case INT_INSERR:
                Halt("Speculator :: Illegal instruction.");
                break;
            case INT_SEGFAULT:
                Halt("Speculator :: Segmentation fault.");
                break;
            case INT_RFE:
                L->valid = true;
                L->EPC_WRITE = true;
                L->priv = false;
                L->epc = 0;
                pc_buf->valid = true;
                pc_buf->jtarget = epc;
                pc_buf->pPort = JUMP;
                break;
        }
    }
    if(EX_MEM.exited)
        exit(0);
}

void Speculator::WriteBack(){
    uint32_t val;

    if (!MEM_WB.valid) return;
    if (MEM_WB.RegWrite)
    {
        val = (MEM_WB.MemToReg == Read_Data) ? MEM_WB.MEM_OUT : MEM_WB.ALU_OUT;
        setReg(MEM_WB.RegDst, val);
    }
    else if (MEM_WB.EPC_WRITE)
    {
        epc = MEM_WB.epc;
        priv = MEM_WB.priv;
#ifdef DEBUG
        printf("Updating states epc=%d, priv=%d\n", epc, priv);
#endif
    }
}

void Speculator::Update(struct _pc pc_tmp, struct _IF_ID L1, struct _ID_EX L2, struct _EX_MEM L3, struct _MEM_WB L4){
    if(Hazard.ex_stall)
    {
        ID_EX = (const struct _ID_EX){ 0 };
        EX_MEM = L3;
        MEM_WB = L4;
        Hazard.ex_stall--;
    }
    else
    {
        PC = pc_tmp;
        IF_ID = L1;
        ID_EX = L2;
        EX_MEM = L3;
        MEM_WB = L4;
    }
    if(FLUSH & L_PC)
        PC.valid = false;
    if(FLUSH & L_IF_ID)
        IF_ID = (const struct _IF_ID){ 0 };
    if(FLUSH & L_ID_EX)
        ID_EX = (const struct _ID_EX){ 0 };
    if(FLUSH & L_EX_MEM)
        EX_MEM = (const struct _EX_MEM){ 0 };
    if(FLUSH & L_MEM_WB)
        MEM_WB = (const struct _MEM_WB){ 0 };
    FLUSH = 0;
}

Speculator::Speculator(string kern, string bin) {
    for (int i = 0; i < REG_NUM; i++) R[i] = 0;
    PC.pc = CODE_START;
    PC.valid = true;
    Hazard.ex_stall = 0;
    FLUSH = 0;
    cycle = 0;
    ifstream kernel(kern);
    ifstream binary(bin);
    if (!kernel.is_open() || !binary.is_open()) {
        cerr << "No " << kern << " or " << bin << endl;
    }
    load(kernel, true);
    load(binary, false);
}

void Speculator::AddTreasure(string fl) {
    ifstream flag(fl);
    if (!flag.is_open()) {
        cerr << "No flag file, tell admin" << endl;
        exit(0);
    }
    Memory.AllocMem(0xdeadb000, 1, PERM_READ);
    uint32_t x = 0;
    uint32_t v = 0;
    flag.read((char *)&v, 1);
    Memory.WriteMem(0xdeadbeec, v);
    while (1) {
        v = 0;
        flag.read((char *)&v, 4);
        if(flag.gcount() == 0) break;
        Memory.WriteMem(0xdeadbef0 + x, __bswap_32(v));
        x += 4;
    }
}
