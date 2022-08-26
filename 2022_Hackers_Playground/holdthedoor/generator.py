# %%
import os
import time
import random
import base64
import random
import sys
import math

class RTree(object):
    def __init__(self):
        self.node_count = 0
        self.nodes = []
        self.root = Node(f"Abstract")

    def create_node(self, parent=None):
        node = Node(f"C{self.node_count}", parent)
        self.nodes.append(node)
        self.node_count += 1
        return node

    def generate(self, max_node=10, max_child=5, birthrate=2):
        rancount = 0
        q = [self.root]
        while len(q) > 0:
            n = q.pop(0)

            ran = range(1, max_child + 1)
            weights = [1 for _ in ran]
            weights[0] = 100 - birthrate
            count = random.choices(ran, weights=weights)[0]
            if count != 1:
                rancount += 1

            for i in range(count):
                if self.node_count > max_node:
                    break
                
                n2 = self.create_node(n)
                n.childen.append(n2)
                q.append(n2)

        print(rancount)

    def print(self):
        print(self.root)
        q = [self.root]
        while len(q) > 0:
            n = q.pop(0)
            for child in n.childen:
                print(child, end=" ")
                q.append(child)
            print()

    def get_children(root):
        res = []
        q = [root]
        while len(q) > 0:
            n = q.pop(0)
            for child in n.childen:
                res.append(child)
                q.append(child)

        return res


class Node(object):
    def __init__(self, name, p=None):
        self.name = name
        self.childen = []
        self.nexts = []
        self.fs = None
        self.codes = []
        self.isLast = False
        self.type = None
        self.parent = p
        self.depth = 0 if p is None else p.depth + 1
        self.key = 0


    def __str__ (self):
        pname = "_" if self.parent is None else self.parent.name
        return f"({self.name}|{pname}|{self.depth})"

        
class Func(object):
    def __init__(self, value, parent=None):
        self.value = value
        self.inheritage = False
        self.parent = parent

    def setParent(self, parent):
        self.parent = parent
        if self.parent is None:
            self.inheritage = False
        else :
            self.inheritage = True

    def getValue(self):
        if self.inheritage:
            return self.parent.getValue()
        else:
            return self.value

    def setValue(self, value):
        if self.inheritage:
            self.parent.setValue(value)
        else:
            self.value = value

    def isInheritage(self):
        return self.inheritage 

class ClassTemplate(object):
    Main_fmt = """
    
package SCTF;

import java.util.Scanner;

class Main {{
    public static void main(String[] args){{
        Abstract start = new {}();
        StringBuilder code = new StringBuilder();
        try{{
            start.getCode(code);
        }} catch(Exception e) {{
            e.printStackTrace();
        }}
        System.out.println(String.format("FLAG=%s",code));
    }}
}}

abstract class Abstract {{
    protected static Scanner scanner = new Scanner(System.in);
    
    public void f0(StringBuilder code) throws Exception {{
        code.append("");
    }}
    public void f1(StringBuilder code) throws Exception {{
        code.append("");
    }}
    public void f2(StringBuilder code) throws Exception {{
        code.append("");
    }}
    public void f3(StringBuilder code) throws Exception {{
        code.append("");
    }}
    public void f4(StringBuilder code) throws Exception {{
        code.append("");
    }}

    public abstract Abstract getNext(Abstract abs);
    public abstract void getCode(StringBuilder code) throws Exception ;
}}

"""

    def __init__(self, node):
        self.node = node

    def getMain(self):
        return ClassTemplate.Main_fmt.format(self.node.name)

    def getFS(self):
        fs = self.node.fs 
        fmt = ""
        for f in fs:
            content = ""
            if fs[f].isInheritage():
                continue
            elif fs[f].getValue() is None:
                content = """throw new Exception("Nope!");"""
            else : 
                # content = f"""return code.concat("{fs[f]}");"""
                content = f"""code.append("{fs[f].getValue()}");"""
            fmt += f"""
    @Override
    public void f{f}(StringBuilder code) throws Exception {{
        {content}
    }}
        """
        return fmt

    def getCondition(self, next, key):
        nonce = next.nonce

        if key == 0: # dead link
            randkey = random.randint(-0xFFFFFFFF + 1, 0xFFFFFFFF)
            if nonce < randkey:
                return f"{random.randint(-0xFFFFFFFF + 1, nonce)}L > key && key * key - key * {nonce + randkey}L == {-nonce * randkey}L"
            else: #  randkey < nonce
                return f"{random.randint(nonce, 0xFFFFFFFF)}L < key && key * key - key * {nonce + randkey}L == {-nonce * randkey}L"
        else:
            if nonce < key:
                return f"{random.randint(nonce, key)}L < key && key * key - key * {nonce + key}L == {-nonce * key}L"
            else:
                return f"{random.randint(key, nonce)}L > key && key * key - key * {nonce + key}L == {-nonce * key}L"


    def getNext(self):
        nexts = self.node.nexts
        input_fmt = ""
        next_fmt = ""

        if len(nexts) != 0:
            input_fmt = f"""        System.out.println("{self.node.name} -> ?\\nkey = ");
            long key = scanner.nextLong();\n"""

        for next, key in nexts:
            next_fmt += f"""        if({self.getCondition(next, key)}) \n """
            if  next.type == "next":
                next_fmt += f"""            next = new {next.name}().getNext(next);\n"""
            else:
                next_fmt += f"""            next = new {next.name}();\n"""

        fmt = f"""
    @Override
    public Abstract getNext(Abstract next) {{
{input_fmt}
{next_fmt}
        return next;
    }}
        """
        return fmt


    def getCodes(self, isTerminal: bool):
        codes = self.node.codes
        code_fmt = ""
        for fnum, type in codes:
            if type == 0:
                code_fmt += f"        this.f{fnum}(code);\n"
            elif type == 1:
                code_fmt += f"        next.f{fnum}(code);\n"

        if not isTerminal:
            code_fmt += "        next.getCode(code);\n"

        fmt = f"""
    @Override
    public void getCode(StringBuilder code) throws Exception {{
        Abstract next = getNext(this);
{code_fmt}
    }}
        """
        return fmt

    def getClass(self):
        node = self.node
        FS = self.getFS()
        next = self.getNext()
        if len(node.nexts) == 0 or node.isLast:
            code = self.getCodes(True) # [(i, random.randint(0, 1)) for i in range(5) if random.randint(0,5) <= 3],
        else:
            code = self.getCodes(False) # [(i, random.randint(0, 1)) for i in range(5) if random.randint(0,5) <= 3]

        fmt = f"""
class {node.name} extends {node.parent.name}{{
{FS}
{next}
{code}
}}
"""
        return fmt



def create_nodes(max_count, width):
    if len(sys.argv) == 2:
        seed = int(sys.argv[1])
    else:
        seed = int(time.time())
    seed = 1657516682
    random.seed(a=seed)
    print(f"Seed: {seed}")

    tree = RTree()
    tree.generate(max_count, width)
    print(f"Node count: {tree.node_count}")
    print(f"Node info: depth: {max_count}, width: {width}")
    return tree.nodes

def setup_nodes(nodes):
    TABLE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
    random.shuffle(nodes)
    node_count = len(nodes)
    # nodes = random.sample(nodes, node_count)

    # setup nodes
    for n in nodes:
        n.fs  = {k:Func("".join(random.choices(TABLE,k=48))) for k in range(5)}
        n.codes = [(k, random.randint(0, 1)) for k in range(5)]
        n.nonce = random.randint(-0xFFFFFF + 1, 0xFFFFFF)
    # setting flag at the first nodes
    for i in range(node_count):
        jMask = random.randint(1, 2 ** 4 - 1)   
        for j in range(4):
            if jMask & (1 << j) != 0 and i + j + 1 < node_count:
                nodes[i].nexts.append((nodes[i + j + 1], random.randint(-0xFFFFFFFF + 1, 0xFFFFFFFF))) # key

    return nodes

def setup_flags(nodes: list, flag: str):
    print(f"flag node length: {len(flag)} {flag}")
    # setup fs randomizing 
    for node in nodes: 
        if node.name == "C0":
            continue

        for f in node.fs:
            ch = random.randint(0, 3)
            if ch == 0:
                node.fs[f].setParent(node.parent.fs[f])
            elif ch == 1:
                node.fs[f].setValue(None)
    # setup code randomizing 
    for node in nodes: 
        tpop = []
        for i, code in enumerate(node.codes):
            ch = random.randint(0, 2)
            if ch <= 1 and len(tpop) < 4:
                tpop.append(i)

        for i in tpop[::-1]:
            node.codes.pop(i)

    # setting flag node
    fi = 0

    cur = nodes[0]
    flag_nodes = []
    flag_nodes.append(cur)
    check = {cur: {}}
    while True:
        next, _ = random.choice(cur.nexts)
        if check.get(next) is None:
            check[next] = {}

        flag_nodes.append(next)      
        tpop = []
        for i, tcode in enumerate(cur.codes):
            code, type = tcode
            # processing this. 
            if type == 0:
                # 중복 제거 코드
                if check[cur].get(code) is not None:
                    tpop.append(i)
                    continue
                if fi < len(flag):
                    cur.fs[code].setValue(flag[fi])
                else: # flag 외에 마지막거는 다 제거
                    tpop.append(i)

                fi += 1
            # processing next. 
            elif type == 1:
                if fi < len(flag):
                    next.fs[code].setValue(flag[fi])
                else: # flag 외에 마지막거는 다 제거
                    tpop.append(i)
                
                check[next][code] = True
                fi += 1
        
        for i in tpop[::-1]:
            cur.codes.pop(i)

        if fi >= len(flag):
            cur.isLast = True
            cur.name = "Last"
            break

        cur = next

    # if nexts has two flags
    for i in range(len(flag_nodes) - 1):
        for j, (next, k) in enumerate(flag_nodes[i].nexts):
            if next in flag_nodes and next != flag_nodes[i + 1]:
                flag_nodes[i].nexts[j] = (next, 0)

    for i in range(len(flag_nodes) - 1): 
        for n, key in flag_nodes[i].nexts:
            if n in flag_nodes:
                print(f"{n.name} | {key}")
                break

    # kill flag node's nexts 
    for node in flag_nodes:
        for next, _ in node.nexts:
            if not next in flag_nodes:
                eliminate_path(next, flag_nodes)

    
    cnt = check_path(flag_nodes[0])

    if cnt != 1:
        raise Exception("Cycle error")

    return 

def eliminate_path(node, flag_nodes):
    if len(node.nexts) == 0:
        print(f"eliminate_path: is terminal {node.name}")
        return
    
    ## getCode check
    tocheck_nextfs=[]
    for code, type in node.codes:
        if type == 0 and node.fs[code].getValue() is None:
            return
        elif type == 1:
            tocheck_nextfs.append(code)
             
    tonext = []
    for next, _ in node.nexts:
        isValid = True
        for next_fs in tocheck_nextfs:
            if next.fs[next_fs].getValue() is None:
                isValid = False
                break
        
        if isValid:
            tonext.append(next)

    if len(tonext) == 0:
        return

    ## getNext check
    for next in tonext:
        if next in flag_nodes:
            print(f"what a lucky: {node.name} -> {next.name}")
            setDead(node, next)
            # next.isDead = True
            # c, _ = node.codes[-1]
            # node.fs[c].setValue(None)    
            # node.codes[-1] = (c, 0) # current throw exception
        else:
            eliminate_path(next, flag_nodes)
        
    return

def setDead(node, todead):
    for i in range(len(node.nexts)):
        if node.nexts[i][0] == todead:
            node.nexts[i] = (node.nexts[i][0], 0)
            break


ss = []
def check_path(node):
    global ss
    if node.name == "Last":
        print("is Last")
        for line in ss:
            print(line.name)
        return 1
    tocheck_nextfs=[]
    for code, type in node.codes:
        if type == 0 and node.fs[code].getValue() is None:
            return 0
        elif type == 1:
            tocheck_nextfs.append(code)

    tonext = []
    for n, k in node.nexts:
        c = True
        if k == 0:
            continue
        for nfs in tocheck_nextfs:
            if n.fs[nfs].getValue() is None:
                c = False
                break
        if c:
            tonext.append(n)
    
    cnt = 0
    for next in tonext:
        ss.append(next)
        cnt += check_path(next)
        ss.pop()

    return cnt

def main():
    NODE_COUNT = 2000
    WIDTH = 3
    SRC = "src/SCTF"
    with open("./flag.jpg", "rb") as fp:
        raw_flag = fp.read()
        FLAG = base64.b64encode(raw_flag).decode()
        FLAG = [FLAG[i * 48: i * 48 + 48] for i in range(math.ceil(len(FLAG)/48))]
    
    nodes = create_nodes(NODE_COUNT, WIDTH)
    setup_nodes(nodes)
    setup_flags(nodes, FLAG)
    
    os.system(f"rm -rf {SRC}/*")
    with open(f"{SRC}/Main.java", "w") as fp:
        fp.write(ClassTemplate(nodes[0]).getMain())

    for node in nodes:
        with open(f"{SRC}/{node.name}.java", "w") as fp:
            fp.write("package SCTF;\n")
            fp.write(ClassTemplate(node).getClass())
            fp.write("\n")

main()

# %%
