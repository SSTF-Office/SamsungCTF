import React from 'react';
import { Redirect } from 'react-router-dom';

import { signup } from '../service/User.js';

class Signup extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      username: '',
      password: ''
    };

    this.handleChange = this.handleChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
  }

  handleChange(e) {
    const { name, value } = e.target;
    this.setState({ [name]: value });
  }

  handleSubmit(e) {
    e.preventDefault();
    const { username, password } = this.state;
    signup(username, password)
      .then(result => {
          window.toast(result.msg);
          this.setState({ redirect: true });
      })
      .catch(error => {
        const reason = error.response.data.reason;
        if (reason) {
          window.toast(reason);
        }
      });
  }

  componentDidMount() {
    window.initTooltip();
  }

  render() {
    if (this.state.redirect) {
      return (<Redirect to="/signin" />);
    }
    const username = this.state.username;
      return (
        <center>
        <h4 className="header">Sign Up</h4>
        <p className="indigo-text">Sign up into R-diary to write your diary</p>
        <div className="section"></div>
        <div className="container">
        <div className="z-depth-1 grey lighten-4 row" id="input_container">
        <div>
        <img src="/static/img/pencil.png" className="center" alt="#" style={{width: "110px", heigth: "110px"}} />
        </div>
        <form className="col s12" onSubmit={this.handleSubmit}>
        <div className='row'>
        <div className='col s12'>
        </div>
        </div>

        <div className='row'>
        <div className='input-field col s12'>
        <input className='validate' type='text' value={username} name='username' onChange={this.handleChange} />
        <label htmlFor='username'>Enter username</label>
        </div>
        </div>

        <div className='row'>
        <div className='input-field col s12'>
        <input className='validate' type='password' name='password' onChange={this.handleChange} />
        <label htmlFor='password'>Enter password</label>
        </div>
        </div>

        <br />
        <center>
        <div className='row'>
        <button type='submit' name='btn_login' className='col s12 btn btn-large waves-effect'>Login</button>
        </div>
        </center>
        </form>
        </div>
        </div>
        </center>
      )
    }
  }

export default Signup;
