# JwtDecoder
Challenge problem for SCTF(Hacker's playground) 2022.

## Author
Einstrasse

## Category
Web

## Challenge Intention
- Known but not patched vulnerability for wide-spread node module
- Not widely known feature for wide-spread node module

## Challenge Structure
Challenge provides full source code of node server. User should trigger arbitrary code execution to get the flag.
Challenge should consider side-effect between users, because its intended vulnerabiliy category is RCE.


## Challenge Paragraph
I am studying nodejs web programming. I wrote simple JWT decode web site with popular node packages. Using recent packages, I certain that there is no severe security issue!

## Deploy
```bash
cd prob
docker-compose down && docker-compose up -d
```
Open web browser and access to http://localhost:3000
- attachment required : `prob.zip`
  
## Flag
`SCTF{p0pul4r_m0dule_Ar3_n0t_4lway3_s3cure}`

## Trouble shooting

Docker base image issue
- Currently working image (node:latest => `docker pull node@sha256:a6f295c2354992f827693a2603c8b9b5b487db4da0714f5913a917ed588d6d41` )

## References
- [https://www.npmjs.com/package/cookie-parser](https://www.npmjs.com/package/cookie-parser)
- [https://securitylab.github.com/advisories/GHSL-2021-021-tj-ejs/](https://securitylab.github.com/advisories/GHSL-2021-021-tj-ejs/)
