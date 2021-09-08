import React from 'react';
import { Redirect } from 'react-router';

class Logout extends React.Component {
  componentWillMount() {
      this.props.app.logout();
  }
  render() {
    return (
      <Redirect to="/" />
    )
  }
}

export default Logout;
