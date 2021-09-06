from wtforms import Form,BooleanField,TextField,PasswordField,validators,ValidationError

def input_sanitization(form,field):
    wrong_chars = "`!#$%^&*()+=[{]};:'/?,<>\\|\""
    for c in field.data:
        if c in wrong_chars: raise ValidationError("Illegal characters!")

class LoginForm(Form):
    username = TextField('Username',[validators.Length(min=4,max=20)])
    password = PasswordField('Password',[validators.Length(min=1,max=20)])

class RegistrationForm(Form):
    username = TextField('Username',[validators.Length(min=4,max=20),input_sanitization])
    email = TextField('Email Address',[validators.Length(min=4,max=20),input_sanitization])
    password = PasswordField('New Password',[
        validators.Required(),
        validators.EqualTo('confirm',message='Passwords must match!')
    ])
    confirm = PasswordField('Repeat password')
    accept_tos = BooleanField('I accept all terms',[validators.Required()])
