pragma solidity ^0.4.18;

contract SCTFBank{
    event LogBalance(address addr, uint256 value);
    mapping (address => uint256) private balance;
    uint256 private donation_deposit;
    address private owner;

    //constructor
    constructor() public{
        owner = msg.sender;
    }
    
    //logging balance of requested address
    function showBalance(address addr) public {
        emit LogBalance(addr, balance[addr]);
    }

    //withdraw my balance
    function withdraw(uint256 value) public{
        require(balance[msg.sender] >= value);
        msg.sender.call.value(value)();
        balance[msg.sender] -= value;
    }
    
    //transfer my balance to other
    function transfer(address to, uint256 value) public {
        require(balance[msg.sender] >= value);
        balance[msg.sender] -= value;
        balance[to]+=value;
    }

    //transfer my balance to others
    function multiTransfer(address[] to_list, uint256 value) public {
        require(balance[msg.sender] >= value*to_list.length);
        balance[msg.sender] -= value*to_list.length;
        for(uint i=0; i < to_list.length; i++){
            balance[to_list[i]] += value;
        }
    }
    
    //donate my balance
    function donate(uint256 value) public {
        balance[msg.sender] -= value;
        donation_deposit += value;
    }

    //Only bank owner can deliver donations to anywhere he want.
    function deliver(address to) public {
        require(tx.origin == owner);
        to.transfer(donation_deposit);
        donation_deposit = 0;
    }
    
    //balance deposit, simple fallback function
    function () payable public {
        balance[msg.sender]+=msg.value;
    }
}
//END

