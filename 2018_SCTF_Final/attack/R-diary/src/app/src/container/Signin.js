import React from 'react';

import { signin } from '../service/User.js';

class Signin extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      username: '',
      password: '',
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
    signin(username, password)
      .then(result => {
        this.props.app.login(result.data);
      })
      .catch(error => {
        const reason = error.response.data.reason;
        if (reason) {
          window.toast(reason);
          this.setState({
            username: this.state.username,
            password: ''
          });
        }
      });
  }

  componentDidMount() {
    window.initTooltip();
  }

  render() {
    const username = this.state.username;
    const password = this.state.password;
      return (
        <center>
        <h4 className="header">Sign In</h4>
        <p className="indigo-text">Please login before writing your diary</p>
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
        <label htmlFor='username'>Enter your username</label>
        </div>
        </div>

        <div className='row'>
        <div className='input-field col s12'>
        <input className='validate' type='password' value={password} name='password' onChange={this.handleChange} />
        <label htmlFor='password'>Enter your password</label>
        </div>
        <label style={{float: "right"}}>
        <a className='pink-text tooltipped' data-position="right" data-tooltip="Then bye!" href='#!'><b>Forgot Password?</b></a>
        </label>
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

export default Signin;
