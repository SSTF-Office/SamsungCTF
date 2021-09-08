import React from 'react';
import { Link } from 'react-router-dom';

import Spinner from '../component/Spinner.js';

import { getDiaries } from '../service/Diary.js';

class View extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      data: null
    };
  }

  componentDidMount() {
    //data: {'title': 'ddddd', 'date': '2018-02-04', 'author': 'admin', 'body': 'Hi! hello'}
    getDiaries(this.props.computedMatch.params.did)
    .then(result => {
      this.setState({ data: result.data.data });
    })
    .catch(error => {
        const reason = error.response.data.reason;
        if (reason) {
          window.toast(reason);
        }
        this.setState({ redirect: true });
    });
  }

  render() {
    if (this.state.redirect) {
      return (<Link to="/list" />);
    }
    else if (this.state.data === null)  {
      return (<Spinner />);
    } else {
      return (
        <div className="container">
          <div className="row">
            <div className="col s10 offset-s1">
              <h4 className="header">{this.state.data.title}</h4>
              <span className="right">{this.state.data.date}</span>
              <span className="left">{this.state.data.author}</span>
              <br />
              <hr />
                { this.state.data.body.split("\n").map((line, idx) => (
                  <span key={idx}>{line}<br /></span>
                ))}
            </div>
          </div>
        </div>
      );
    }
  }
}

export default View;
