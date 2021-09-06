# longest-substr

## Description
Find a longest sub-string from long-long one-line input(max 130000 char). If there are same length candidates, print only the first one in ascending order.

### SAMPLE INPUT 1
```
wasdfasdfwasdfwasdf<EOF>
```
### SAMPLE OUTPUT 1
```
asdfwasdf<EOF>
```

### SAMPLE INPUT 2
```
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed finibus diam nisl, at tristique sapien tristique et. Morbi quis neque nec ante venenatis auctor in ut nunc. Maecenas vitae facilisis elit, eu cursus risus. Mauris et metus fermentum, tincidunt lectus ac, tincidunt erat. Ut cursus finibus nibh, ac vehicula enim suscipit ut. Vivamus eu ullamcorper lectus. Curabitur fringilla non ex vel finibus. Donec vel mollis dolor. Morbi sodales interdum posuere. In eu tempus nisl, in dictum metus. Suspendisse potenti. Ut ut ante porttitor, vehicula lacus rhoncus, placerat arcu<EOF>
```
### SAMPLE OUTPUT 2
```
, tincidunt <EOF>
```

## Comment
Let N be a length of input. We can store N substrings that are made from each character of input to end of input, and sort them. You need to check only subsequent substring pairs.

