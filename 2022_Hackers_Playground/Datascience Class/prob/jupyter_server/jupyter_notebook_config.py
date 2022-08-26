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
