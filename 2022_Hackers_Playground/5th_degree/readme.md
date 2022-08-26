## 5th degree

### Overview

| Type              | Contents         |
|:-----------------:|:-----------------|
| Difficulty        | Easy             |
| Category          | Coding/Misc      |
| Tags              | math             |
| Author            | matta            |

### Description

> It's highschool math.
> 
> nc challenges.ssat.site 1337

### Deployment

  ```
  cd deploy/server
  docker-compose down
  docker-compose up -d --build
  ```

  * `flag` is in the /deploy/server/docker-compose.yml.

### Solving strategy

1. Take coefficients
2. Differentiate the polynomial
3. Rind roots by numpy, sage, or something
   * numpy cannot get accurate roots, so correction is necessary.
4. find minumum and maxmum from l and m, and roots between l and m.
