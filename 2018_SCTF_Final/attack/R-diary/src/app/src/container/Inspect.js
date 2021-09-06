import React from 'react';

import { inspectDiaries } from '../service/Diary.js';

class Inspect extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      url: '',
      time: '',
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
    const { url, time } = this.state;
    inspectDiaries(encodeURIComponent(url), time)
      .then(result => {
        if (result.status === 200) {
          window.toast(result.data.msg);
          this.setState({ url: '', time: '' });
        } else {
          window.toast(result.error);
        }
      })
      .catch(error => {
        const reason = error.response.data.reason;
        if (reason) {
          window.toast(reason);
          this.setState({
            url: '',
            target: ''
          });
        }
      });
  }

  componentDidMount() {
    window.initShareForm();
  }

  render() {
    const { url, time } = this.state;
      return (
        <center>
        <h4 className="header">Inspect</h4>
        <p className="indigo-text">Did you find bad diary?<br />
        Our super genius bot will inspect bad diary.</p>
        <div className="section"></div>
        <div className="container">
        <div className="z-depth-1 grey lighten-4 row" id="input_container">
        <form className="col s12" onSubmit={this.handleSubmit}>
        <div className='row'>
        <div className='col s12'>
        </div>
        </div>

        <div className='row'>
        <div className='input-field col s12'>
        <input className='validate' type='text' value={url} name='url' onChange={this.handleChange} />
        <label htmlFor='url'>url</label>
        </div>
        </div>
        <div className='row'>
        <div className='input-field col s12'>
        <input className='validate' type='text' value={time} name='time' onChange={this.handleChange} />
        <label htmlFor='time'>Duration</label>
        </div>
        </div>
        <br />
        <center>
        <div className='row'>
        <button type='submit' name='btn_login' className='col s12 btn btn-large waves-effect'>Inspect</button>
        </div>
        </center>
        </form>
        </div>
        </div>
        </center>
      )
    }
  }

export default Inspect;
