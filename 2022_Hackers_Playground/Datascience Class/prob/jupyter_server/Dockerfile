FROM jupyterhub/jupyterhub

RUN pip install pip -U
RUN pip install jupyter-server notebook==6.4.0 jupyterlab jupyterhub-nativeauthenticator

RUN mkdir -p /etc/jupyter
RUN echo "umask 027" >> /etc/profile
RUN echo "umask 027" >> /etc/bash.bashrc

RUN groupadd admin
RUN useradd -rm -d /home/admin -s /bin/bash -g admin admin -p "$(openssl passwd -1 jupyt3rhub_adm1n)"
RUN useradd -rm -d /home/sub-admin -s /bin/bash -g admin sub-admin -p "$(openssl passwd -1 3ub_adm1n)"

COPY ./flag /home/admin/flag
RUN chown admin:admin /home/admin/flag
RUN chmod 640 /home/admin/flag

ENV JUPYTERHUB_SINGLEUSER_APP 'notebook.notebookapp.NotebookApp'

COPY ./jupyter_notebook_config.py /etc/jupyter/jupyter_notebook_config.py

CMD ["jupyterhub", "-f", "/etc/jupyter/jupyter_notebook_config.py"]

