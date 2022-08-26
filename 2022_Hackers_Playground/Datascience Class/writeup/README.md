# Additional Description

## How to find flag file ?

- Use terminal in jupyterhub, and use find command. admin owns it

## How to find who is reading assignment.ipynb ?

- You can find ```jupyter_notebook_config.py``` at ```/etc/jupyter``` and there is configuration for ```sub-admin``` which allow to read other users' work
```python
:/etc/jupyter$ cat jupyter_notebook_config.py
import os
os.umask(0o0027)

c.Authenticator.admin_users = {'admin'}
c.LocalAuthenticator.create_system_users = True
c.JupyterHub.authenticator_class = 'nativeauthenticator.NativeAuthenticator'
import nativeauthenticator
c.JupyterHub.template_paths = [f"{os.path.dirname(nativeauthenticator.__file__)}/templates/"]
c.NativeAuthenticator.open_signup = True

c.JupyterHub.load_roles = [
    {
        'name': 'server-rights',
        'description': 'permission to access server',
        'scopes': ['list:users', 'access:servers'],
        'users': ['sub-admin']
    }
]

import subprocess

def pre_spawn_hook(spawner):
    username = spawner.user.name
    try:
        subprocess.check_call(['useradd', '-ms', '/bin/bash', username])
    except Exception as e:
        print(e)

c.Spawner.pre_spawn_hook = pre_spawn_hook
```

## How to know sub-admin can read flag ?

- Need to lookup ```/etc/passwd```
```bash
:/etc/jupyter$ cat /etc/passwd | grep admin
admin:x:999:1000::/home/admin:/bin/bash
sub-admin:x:998:1000::/home/sub-admin:/bin/bash
```
- You can see that sub-admin is in same group in admin
