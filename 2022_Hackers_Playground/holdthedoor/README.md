## Hold The Door

### Overview

| Type              | Contents            |
|:-----------------:|:--------------------|
| Difficulty        | Low/Medium          |
| Category          | Reversing           |
| Tags              | Reversing           |
| Author            | won0                |

### Description
 > open the correct doors. 


### Deployment
 deploy/sctf.jar

### Solving strategy
the purpose is to test java staic analysis skills.
there are 2000 classes and each class has methods "getCode", "getNext", "f0", "f1", "f2", "f3", "f4"

1. extract the class information and then switch to the graph problem.
2. remove the next node if the equation doesn't have a correct value.
3. remove the node where an exception is thrown in the f* functions.
4. if you find the correct route, decode the printed result from base64 to binary format.
5. the flag is a jpg file.

> **_NOTE:_** <br />
> in fact, this challenge was planned to be used only as a qualifier for a next challenge at first.<br />
> you could simply try all combination of the paths without calculating and it may not take much time.<br />


#### Connection Types
* class hierarchy connection


```java

C2 -> C1
C3 -> C2
C4 -> C3


class C2 extends C1 {...}
class C3 extends C2 {...}
class C4 extends C3 {...}
... 

```

* functional connection

```java
C1307->C1579
C1307->C1411

class C1307 extends C1267{
    ...
    public Abstract getNext() {
        Abstract next = null;
        System.out.println("C1307 -> ?\nkey = ");
            long key = scanner.nextLong();

        if(-543645544L > key && key * key - key * 3163524646L == -21817864070178600L) 
             next = new C1579();
        if(609775618L < key && key * key - key * 1187516082L == 2379780436493880L) 
             next = new C1411();

        return next;
    }
```




<br />
Required knowledge: Java static analysis, SMT Solver<br />



* keys
```
-4057502211
-2795059730
3433460320
2250960384
324970275
977738619
-2348313193
-1193320
2054952525
-1333640173
4243112968
-2252028102
-987585248
-2136127110
513396791
-1737424914
3001485137
-1896186079
1875470051
3304405005
-3058943158
1958262062
-2206546945
-3438164764
-2163855347
1371233518
4169640478
1266441430
-2169237285
3169491527
-1982758482
3766700469
-1523702044
-3030161265
3104706514
-3851061317
-474689421
-2271456590
3245518423
-2827939675
-361817838
3514502959
-3191190919
1136450732
1526948933
-450574748
-3003437776
4209276009
-230713085
-3220153819
-2076780758
-2298346536
-1829841032
-1100251617
-1411151907
2232583862
3224335211
1424738718
3333380151
3963646588
1780268901
-4022861148
-3815533185
896378139
677689235
839908595
-763265581
-3819791467
3635337378
-1806584349
-3689059070
-2294835460
-2677071267
-141940598
352529299
2146977620
-2759817646
1818844658
3658333801
-77820561
3332718991
-1962410534
-3090452416
-133363483
-4077639522
1533707883
6073356
1048932083
-1816728858
-2875755913
```

```python
import base64

a = ['/9j/4AAQSkZJRgABAQEAYABgAAD/2wBDAAIBAQIBAQICAgIC', 'AgICAwUDAwMDAwYEBAMFBwYHBwcGBwcICQsJCAgKCAcHCg0K', 'CgsMDAwMBwkODw0MDgsMDAz/2wBDAQICAgMDAwYDAwYMCAcI', 'DAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwMDAwM', 'DAwMDAwMDAwMDAwMDAz/wAARCAAaAQYDASIAAhEBAxEB/8QA', 'HwAAAQUBAQEBAQEAAAAAAAAAAAECAwQFBgcICQoL/8QAtRAA', 'AgEDAwIEAwUFBAQAAAF9AQIDAAQRBRIhMUEGE1FhByJxFDKB', 'kaEII0KxwRVS0fAkM2JyggkKFhcYGRolJicoKSo0NTY3ODk6', 'Q0RFRkdISUpTVFVWV1hZWmNkZWZnaGlqc3R1dnd4eXqDhIWG', 'h4iJipKTlJWWl5iZmqKjpKWmp6ipqrKztLW2t7i5usLDxMXG', 'x8jJytLT1NXW19jZ2uHi4+Tl5ufo6erx8vP09fb3+Pn6/8QA', 'HwEAAwEBAQEBAQEBAQAAAAAAAAECAwQFBgcICQoL/8QAtREA', 'AgECBAQDBAcFBAQAAQJ3AAECAxEEBSExBhJBUQdhcRMiMoEI', 'FEKRobHBCSMzUvAVYnLRChYkNOEl8RcYGRomJygpKjU2Nzg5', 'OkNERUZHSElKU1RVVldYWVpjZGVmZ2hpanN0dXZ3eHl6goOE', 'hYaHiImKkpOUlZaXmJmaoqOkpaanqKmqsrO0tba3uLm6wsPE', 'xcbHyMnK0tPU1dbX2Nna4uPk5ebn6Onq8vP09fb3+Pn6/9oA', 'DAMBAAIRAxEAPwD9/KK8O/bz8H+NvGPw30CLwnZeLNc0m212', 'KfxVofhTxEPDviDW9M8mZfJsb8z23kyJdNazMBdWxkiglTzf', 'mMUvkvhvW9J/aL8WfDT4V+F/HPxQ0z4a3Wga7reqfadZ1XTf', 'Ft/cWF/ZWw0q5v52TVbb7O94XlxKty+yBWlMTOsxTvOXJHe7', '+5R5m/Sydnqm9N9BztCKnLb/ADdkvW/po09uZx+y6K+NP26f', '2TtJ+H/7PHhyTTPG3xvtp9N8YeGtGtZ4fiv4kt5xY6h4j0+3', 'uoJZYr5Hut0VxMiy3LSzIr4SRQiBeZ+M/wAOH0D9tTTvAlj4', 'a/aa+IXhfw94F0y5tYPCnxgv9NbS5pdT1ESXN/Nd69ZTXpdU', 'RQXa5cLDtwo2guC5nHzk4/dHnd/k0u1762SbJJxpyq9Ir/26', 'Mf1v3t0vofeNFfBHxe13xf4y8WaFrvhfUPt2vaj8fP7J0ZPE', 't5cPYaHb22g3tqWNirq8YjYXFx5A8iS6yhMqLKky+iXPjXx1', '8W/hz8TPA/j/AELxR448SfDfxFZWkx+EGsP4LuPFFtcWkF3C', '0b3Gqwy2Ri+0fvYhqWXFurBmWYwVEJc1P2i8vxjCX3rnSa7p', '66OxJWnyev4SnH7nyXT81ptf6zor84h8QPiR8ZfBHwW+E3hn', 'VNe8XT3en+JtT1xbrx5qngrUXGk6lb2cOm3+sR2z6uGtvtap', 'JLBCJbqa2iaRxDLKZMb4xfGrxN8a/Adjq3hBvjbPJ4S+GVre', '21to/i6a0m+HmrrcX0Ump688M6LrkWbEYgSLUZXFlOfsb/a1', 'L1eOkr+627PyXNd9t4tb8u15JNDjFyfL1t57tpW2v1TfW2ii', '5aH6bUV8w+Gf2mfiV8WfEfjPxD4QuvAjfDf4azJp93BdaVdX', 'WreL5RptvfS3Vpcx3SRWcYW7iEcb29w0pjYF4Q4ZfC4fGmvf', 'AT4IeAviHp/xa8UDWPi/8Mda1/xBqmv6vf69pFnqI0qPULfV', 'LXT5PNW1jgndoxbWMEcbpOiGJ2WIBTvGMptfCk/P3lJw0/vK', 'L81pdb2ilKNRwinrJtLt7rSle1/hcl5PWzP0Sor89PhJ8dPG', 'v7L2oeKtFttF+I2j6x4htvDGn+GvC/xR8XT+JbltTv8AUJNP', 'n1r7b9puithultt1nHcrKDbEm2s/PR5fU/2p/gB8fvib4H8H', 'iW88L+N77w3d6hcaxaeGfHfiT4TR69G8CC18uSwkvZ1eNvNU', 'wy3DROdkm9M7EqouWLmtUna/S6tdfK6b287NSScGpT5Otk/k', '1dN+u3XVXV1Zv64or4L/AGlf+Cnj/Az9m7wr4w+HepRNo+l+', 'B4PF154e1nwh4h8Z67d2XlkxwXt/YzPHo5dYJUXUL97lJpBM', '2CtvIz63xE/4KTeOY/2oNa0Lwp4e1TUfDvg/xHpnhu+0a3+F', 'XibW7vWzcLaS3V1Hr1oP7L09YIrwMIp0lLfZm3vCJlMdcl58', 'ke7X3NK+l+rW138tRN8sFOel1+l7a26a62Xzuj7eor428O+D', 'vF/wJ/bqTW/iDZa3qeifEXxPqFh4V1rTPi54gu7OxZ7SWa2t', 'Lzw1MItOhT7Pb3AEsJnIlVGKLv8AMTiP2Kvjz8WPgF+zJ+z5', 'qfiSPwHf/DXxdbx+H4tF0ywuRr+kKtjdXEF8189z5F0JBaZe', '3W0hMIuOJ5fIJlxVSPs1UnpdK/k39l+a0W3W+1m9PZy5pRXS', '9vNK/vLy0b+Vvi0P0Aor5Z8FftMfFFP2W5fi54w1v4WaBoXj', 'LRLTVfDGnJoepT3fh2S9aL7JbXBiuJH1q4dJ0CwWkNpJLOqw', 'R584SR8j8KP2itf/AG0f2afiHZ6/oPxI8Y6r8PfGMehXNt8P', 'LfUvhbrHiJDZ2d4jrb6rqFne6esYvV3o18pl+zblZ45fKOso', 'TUpQa96OrXb3lFrS+qbX33TtdqIuMoxqRd4y2ffS6ettH9/l', 'tf7Uor809Zt9a+Jmi/sxHxL468G614I8Iav4hvvEFv4+8L3+', 'sz2cukC6YC7lubq1Z7qyt0MK3Vxbu/nxfasybsNl/Cn/AIKJ', 'yftV/Hz4y6l4N+PXgFbnxT8MpLvwJ4Xttfh1ePQYrSa7YTzW', 'VteRudSmgDXEoV0kgjkt43GYG3c9WvGHNbWynL5Rg5pvdLm5', 'XbXpcqnDmtfS7jH5uUYtd/d5lf7j9QKK/Na9+C+p/s+f8E+f', 'hhqVla+M9Iv9I8Mw65/wlnw2muPDPhLwlIxiupNR1fRbfUpb', 'nV5XVg02+2vlmMUjOtqksrV6Nov7ND/tCf8ABQv406/488Q6', 'De+HdG0PRrfSL7QIdW8OeJNGs5Y5bhYrfW7TVFlgt2Ku08cS', 'IlyTGWCiPa3TiIexm4SfwtpvorfjrZ20S0vcwo1faQjOK+JJ', 'rzv/AJJq/XVKx9x0V+c+l+APE/hz4L/BbxR4K1nXdVvvid8W', 'odd8LweO/FOs+Irfw7pU2i6jFZuWvp5Lt/8ARVF5JbebHvnm', 'kiEkKlXj6z4u/Gb4meKYbnwb8R08TajofgDx3Bp3jnW/g7p+', 's2N7q2lXGjNeWUkNrYz3GrWwW8mtY50tJ5ZCsayF1gllRY7p', '6NO1npraDa9U52fezs3rbZqzS3vHm010vNfjyXXrZ+f3ZRXw', 'J8OfFek/Ev8AY9tbPxx4g+Pt9PZeMvEHh7wHpWl+ItY0Lxj4', 'mSG6mjso7iS1nhupZIIVKPJfSrHEqtLfFZI3kTof2jP2ZdU8', 'S/BX4V6L4q8cfEy1+Ovjm10zwhPq3hn4keItEsYbuO0efUtS', 'jsrG7hszJFBBdyo0kGySUQq4IYKWk2uZdeW19PjV16Nacyek', 'VJNsWl2m9ua/X4HZv0evK7Xdmkj7aor4S8ReMfi78Ffiz+1H', '4l+H0/gBfDnw9n07Wb628T295f33ij7P4csZJLSO5S5hFg3l', 'QjF1Kl5ve4yYVER831Dw3+098TfjD4m8aeJfCtv4R0f4efDi', 'ZbG+0fVtKubrXvEE39m29/M8NzFcpDZiNbuJFje3uDK0TgtC', 'HDLlKrBQlPpGPM/KPd+TenfvZalKErwi95uy85Wu4+qTv287', 'ppfTtFfnnovxsv8A9jn4U/DP4xyeLfGfjyX4jfDzU/FXivT9', 'V8UXl5pup3kenQ6jFcWcNwzxaZGsrvAEtIooRHcrvjYxx7fR', 'fCmg/Fs/tz+FLX4n+IfBWr6nf/CzxG1pdeBtGudFksXe90QS', 'Qqt3d3SyujYMdwXjDZ+aFNuW3nTlCt7CXxLmv6xjOVl3+D7m', 'utk8qdSE6aqwd4u1u9m4q77fEv8Ahk2vseivzk1D4xeL/wBn', '/wCF3xa0Pwz/AMLb8H65q2o+E9N0Xwr468Ut4h8SeHzq+oDT', 'ZdRt9Uvbq/tGWbP7qJLu5iglti7ojyvAep+H+v618I0sNE1i', 'Px34W8RfDL4iaXJqliPinq3jm01aw1SxaILBcXwivL9Gyx+y', 'S25eKeJngQgK5mNpNW2coxv5txT+UeZN2u7X00ZUnyxbe6Un', 'bySbXzlbT89Vf7yooopDOP8AjR8DPD/x98MQ6V4h/tyKK2nF', 'zb3Wi69f6FqFrIAVzFeWM0NxHlWZWCSAMrFWBBIrkde/YS+G', 'HiP4X6J4SuND1SOy8OXkmo6ZqVr4h1K01+yu5d/n3SavFcLq', 'AuJhJIs032jzJllkWRnDsD69RSsvvC7/AK89/wA2eeaf+yx4', 'JsfhTZ+C307Ub/QrLU7TWh/aOtX1/fXF7a3kV7BcT3k8z3Nx', 'Is8ELZllfcI1RsoNtdNbfDbRbT4lXni+Oy2+Ir/TYNHuLvzp', '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']

with open("./zz.png", "wb") as fp:
    fp.write(base64.b64decode("".join(a)))
```


### Flag
 SCTF{b368c08c5ae5bca3f4f3f7b927bc0d36}
