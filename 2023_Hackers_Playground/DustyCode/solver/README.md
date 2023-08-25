# Solutions

## Getting the server source code
First of all, you should find the corresponding source code repo with provided service. There are a lot of repositories exists in zip file.
As there is `.git` directory is provided to each directory, you can access to git commit history.
If you check latest git commit date, all the repository except one has latest commit date as before several years from present.
The fact that there is no git commit recent several years, it means that the repo is not maintained recently, with bunch of dust!
It is feasible way to identify repo currently in use in real working in company.

After find the corresponding repo, you can refer source code of the service and you can find command injection vulnerability easily.
But you will find that applied filter is not bypassable to reach `exec` code. 
Then check the branch of git. You can find the branch is set to not release but develop. Usually branch name matched to pattern like release, master, prod is used to deploy the service.
When you switch git branch to `release` now you finally got code of actuall service.
It is another one of case you should take care, when you are conducting white-box testing with git repository.

## Exploiting service
Now, last stage of challenge solve is just exploiting command injection vulnerability. Even with constraint of JOI library validation to email like format, you can perform command injection.
Just do it!
