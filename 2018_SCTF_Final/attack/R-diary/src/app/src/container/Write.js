import React from "react";
import { Redirect } from "react-router-dom";

import { addDiaries } from '../service/Diary.js';

class Write extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      title: "",
      date: "",
      ispub: false,
      body: ""
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

    addDiaries(this.state)
    .then(result => {
        window.toast(result.data.msg);
        this.setState({redirect: true, id: result.data.id});
      })
    .catch(error => {
      const reason = error.response.data.reason;
      if (reason) {
        window.toast(reason);
      }
      this.setState({
        title: "",
        date: "",
        ispub: false,
        body: ""
      });
    });
  }

  componentDidMount() {
    window.initDiaryForm();
  }

  render() {
    if(this.state.redirect) {
      return (<Redirect to={"/view/" + this.state.id} />);
    }
    const { title, date, body } = this.state
    return (
      <center>
      <h4 className="header">Write Diary</h4>
      <p className="indigo-text">Write your diary</p>
      <div className="section"></div>
      <div className="container">
      <div className="z-depth-1 grey lighten-4 row" id="diary_container">
      <form className="col s12" onSubmit={this.handleSubmit}>
      <div className="row">
      <div className="col s12">
      </div>
      </div>

      <div className="row">
      <div className="input-field col s12">
      <input className="validate" type="text" value={title} name="title" onChange={this.handleChange} />
      <label htmlFor="title">Title</label>
      </div>
      </div>
      <div className="row">
      <div className="input-field col s6">
      <input className="validate datepicker" type="text" value={date} name="date" onChange={this.handleChange} />
      <label htmlFor="date">Date</label>
      </div>
      <div className="col s6">

      <div className="col s2">
      <label className="left"> Privacy </label>
      </div>
      <div className="input-field col s5">
          <p>
            <label className="left">
              <input name="ispub" type="radio" disabled="disabled" />
              <span>Public</span>
            </label>
          </p>
      </div>
      <div className="input-field col s5">
          <p>
            <label className="left">
              <input name="ispub" type="radio" defaultChecked/>
              <span>Private</span>
            </label>
          </p>
      </div>
      </div>
      </div>

      <div className="row">
      <div className="input-field col s12">
      <textarea name="body" type="text" id="body" value={body} className="materialize-textarea" onChange={this.handleChange}></textarea>
      <label htmlFor="body">Textarea</label>
      </div>
      </div>

      <br />
      <center>
      <div className="row">
      <button type="submit" name="btn_login" className="btn btn-floating waves-effect">
        <img src="/static/img/pencil.png" alt="#" style={{"maxWidth": "100%", "maxHeigth": "100%"}} />
      </button>
      </div>
      </center>
      </form>
      </div>
      </div>
      </center>
    )
  }
}

export default Write;
