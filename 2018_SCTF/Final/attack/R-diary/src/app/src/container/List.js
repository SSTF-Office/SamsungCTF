import React from 'react';
import { Link } from 'react-router-dom';

import Spinner from '../component/Spinner.js';

import { listDiaries } from '../service/Diary.js';

class List extends React.Component {
  constructor(props) {
    super(props);

    this.state = {
      data: null
    };
  }

  componentDidMount() {
    listDiaries()
    .then(result => {
        this.setState({ data: result.data.data })
    })
    .catch(error => {
        const reason = error.response.data.reason;
        if (reason) {
          window.toast(reason);
        }
    });
  }

  render() {
    if (this.state.data === null)  {
      return (<Spinner />);
    } else {
      return (
        <div className="container">
          <div className="row">
            <div className="col s10 offset-s1">
              <ul className="collection with-header">
                <li className="collection-header"><h4>Diaries</h4></li>
                { this.state.data.map( entry => (
                  <li key={entry['id']} className="collection-item">
                    <div>{entry['name']}
                      <Link to={'/view/' + entry['id']} className="secondary-content">
                        <i className="material-icons">send</i>
                      </Link>
                    </div>
                  </li>
                ))}
              </ul>
            </div>
          </div>
        </div>
      );
    }
  }
}

export default List;
