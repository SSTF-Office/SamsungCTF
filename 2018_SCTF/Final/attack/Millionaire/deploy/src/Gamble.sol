pragma solidity ^0.4.18;

contract Gamble {
    event Winner(address from, uint value);
    event Looser(address from, uint human, uint bot);
    event Jackpot(address from);
    mapping (address => uint8) private winCounts;
    
    address private owner;

    //constructor
    constructor() public{
        owner = msg.sender;
    }

    function choice() private view returns(uint8) {
        // random value
        return uint8(uint(keccak256(block.timestamp))+uint(keccak256(blockhash(block.number-1)))) %10;
    }

    function play(uint8 human) payable public {
        uint8 bot = choice();
        if (human == bot){
            winCounts[msg.sender] += 1;
            msg.sender.transfer(msg.value*2);
            emit Winner(msg.sender, msg.value*2);
        }
        else{
            winCounts[msg.sender] = 0;
            emit Looser(msg.sender, human, bot);
        }
    }

    function getJackpot() public {
        if (winCounts[msg.sender] > 10){
            // send half of current Jackpot balance! 
            msg.sender.transfer(address(this).balance/2);
            emit Jackpot(msg.sender);
        }
    }

    function () payable public {}
}
