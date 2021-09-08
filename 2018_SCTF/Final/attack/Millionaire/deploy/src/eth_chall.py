import web3

from web3 import Web3, EthereumTesterProvider
from solc import compile_source
from solc.exceptions import SolcError
from web3.contract import ConciseContract
from eth_tester import EthereumTester

import sys
import pprint

# functions for challenge setup
#from config import flag, winput, wprint, check_user 
flag = 'SCTF{flag}'
wprint = print
winput = input

class EthereumWorker:
    def __init__(self):
        # web3.py instance with new test network
        self.eth_test = EthereumTester()
        self.Provider = EthereumTesterProvider(self.eth_test)
        self.w3 = Web3(self.Provider)
        self.default_accounts = self.w3.eth.accounts
    
    # Ethereum Interface functions #
    def make_contract(self, account, contract_source_code, contract_name, **kwargs):
        compiled_sol = compile_source(contract_source_code) # Compiled source code
        contract_interface = compiled_sol['<stdin>:'+contract_name]
        # Instantiate and deploy contract
        contract = self.w3.eth.contract(abi=contract_interface['abi'], bytecode=contract_interface['bin'])
        # Get transaction hash from deployed contract
        tx_hash = contract.constructor(**kwargs).transact({'from': account})
        # Get tx receipt to get contract address
        tx_receipt = self.w3.eth.getTransactionReceipt(tx_hash)
        contract_address = tx_receipt['contractAddress']

        return tx_receipt, contract_address, contract_interface['abi'], contract_interface['bin-runtime']

    def make_account(self, value, sender=None):
        new_account = self.w3.eth.account.create()
        #register private key to testnet
        address = self.Provider.ethereum_tester.add_account(new_account.privateKey.hex())
        #give initial ethereum
        sender = sender if sender else self.default_accounts[0]
        self.send_transaction(sender, address, value)
        assert(self.w3.eth.getBalance(address) == value)
        return address
    
    def send_transaction(self, sender, to, value, gas=90000, data=None):
        try:
            options = {'from':sender, 'to': to, 'value': value, 'gas': gas}
            if data:
                options['data'] = data 
            tx_hash = self.w3.eth.sendTransaction(options)
            tx_receipt = self.w3.eth.getTransactionReceipt(tx_hash)
            return tx_receipt
        except:
            wprint(sys.exc_info()[0])
            return None

    def get_balance(self, addr):
        return self.w3.eth.getBalance(addr)


class EthereumChallenge(EthereumWorker):
    # Functions for Challenge #
    def __init__(self, challenge_file, challenge_name):
        super(EthereumChallenge, self).__init__()

        # compile challenge smart contract
        with open(challenge_file) as f:
            self.contract_source_code = f.read()
        self.contract_name = challenge_name
        try:
            self.contract_receipt, self.contract_address, self.contract_abi, self.contract_bin = self.make_contract(
                self.default_accounts[0], self.contract_source_code, self.contract_name)
            self.contract_receipt = dict(self.contract_receipt)
            self.contract_receipt['logs'] = list(map(lambda x: dict(x),self.contract_receipt['logs']))
        except SolcError as e: 
            errMsg = str(e)
            wprint(errMsg)
            exit()
        # charge enough money on challenge contract
        self.send_transaction(self.default_accounts[1], self.contract_address, self.w3.toWei(500000,'ether'))
        # init user accounts
        self.user_acc = []

    def menu(self):
        wprint('')
        wprint('1: Make Account')
        wprint('2: Show Balance')
        wprint('3: Make Contract')
        wprint('4: Send Transaction')
        wprint('5: Show Block Info')
        wprint('8: Get Flag')
        wprint('9: Show Challenge Info')
        wprint('0: Quit')

    def get_code(self):
        wprint('Give me solidity code, ends with "//END" line\n')
        code =''
        while True:
            line = winput()+'\n' 
            if "//END" in line:
                break
            code += line
        return code

    def do_contract(self):
        # TODO: no winput for constructor yet.
        addr = winput("From which address? ")
        if addr not in self.user_acc:
            wprint("You can make contract from your accounts")
            return
        code = self.get_code() 
        name = winput("Contract name? ")
        try:
            receipt, contract_address, contract_abi, contract_bin = self.make_contract(addr, code, name)
            receipt = dict(receipt)
            receipt['logs'] = list(map(lambda x: dict(x),receipt['logs']))
            wprint("Created at {}".format(contract_address))
            wprint("Here's receipt.\n {}".format(pprint.pformat(dict(receipt))))
            wprint("\nWith ABI {}".format(contract_abi))
            wprint("\nBinary Code {}".format(contract_bin))
            
        except SolcError as e: 
            errMsg = str(e)
            wprint("Compile Error: {}".format(errMsg))
    
    def do_transaction(self):
        addr  = winput("From which address? ")
        if addr not in self.user_acc:
            wprint("You can send transaction from your accounts")
            return
        to    = winput("To which address? ")
        value = int(winput("Value (in wei)? "))
        gas   = int(winput("Gas ()? "))
        data  = winput("Data? (Optional, hex encoded 0x...) ")

        receipt = self.send_transaction(addr, to, value, gas, data)
        if receipt: # need beautifier..
            receipt = dict(receipt)
            receipt['logs'] = list(map(lambda x: dict(x),receipt['logs']))
            wprint("Here's receipt.\n {}".format(pprint.pformat(dict(receipt))))
            if gas and receipt['gasUsed'] == gas:
                wprint("Looks failed, Out of Gas..")
        else:
            wprint("Error in transaction")

    def run(self):
        #check_user()
        wprint('Welcome to Ethereum Challenge!')
        wprint('This is private test network, generated for each connection')
        wprint('Take Money, become millionaire!')

        while True:
            try:
                self.menu()
                cmd = winput("Select? ")
                if cmd == '1': # make account
                    if len(self.user_acc) > 2:
                        wprint('You have enough accounts already!')
                    else:
                        addr = self.make_account(self.w3.toWei(1,'ether'))
                        self.user_acc.append(addr)
                        wprint('{} created with 1 ether!'.format(addr))

                elif cmd == '2': # show balance
                    addr = winput("Address? ")
                    wprint("Current Balance: {} wei".format(self.get_balance(addr)))

                elif cmd == '3': # make contract
                    self.do_contract()
                    
                elif cmd == '4': # send transaction
                    self.do_transaction()

                elif cmd == '5': # show block info
                    wprint("Not Implemented Yet :(")
                
                elif cmd == '8':
                    for acc in self.user_acc:
                        if self.get_balance(acc) > self.w3.toWei(100000,'ether'):
                            wprint("Wow! You are millionaire!")
                            wprint("Flag: {}".format(flag))
                            return
                    wprint("You need 100000 ethers to get the flag")
                
                elif cmd == '9': # show challenge info
                    wprint("Challenge Contract at {}\n".format(self.contract_address))
                    wprint(self.contract_source_code)
                    wprint("\nCreation receipt {}".format(pprint.pformat(dict(self.contract_receipt))))
                    wprint("\nWith ABI {}".format(self.contract_abi))
                    wprint("\nBinary Code {}".format(self.contract_bin))

                elif cmd == '0':
                    return
                else:
                    wprint("Unknown Command")
            except:
                wprint("Error {}".format(sys.exc_info()[0]))


if __name__ == '__main__':
    if len(sys.argv) < 3:
        wprint("Usage: {} [solidity filename] [contract name]".format(sys.argv[0]))
    else:
        ec = EthereumChallenge(sys.argv[1], sys.argv[2])
        ec.run()
