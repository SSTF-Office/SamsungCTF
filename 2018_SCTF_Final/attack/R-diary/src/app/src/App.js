import React from 'react';
import { Route, Switch } from 'react-router-dom';

import { AuthRoute, UnAuthRoute } from './component/RouteHelper';

import Error404 from './container/Error404.js';
import Home from './container/Home.js';
import Header from './container/Header.js';
import Signin from './container/Signin.js';
import Signup from './container/Signup.js';

import View from './container/View.js';
import Inspect from './container/Inspect.js';
import List from './container/List.js';
import Write from './container/Write.js';
import Logout from './container/Logout.js';


class App extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      isAuthed: localStorage.getItem('user') !== null,
      username: localStorage.getItem('user'),
      token: localStorage.getItem('token'),
      app: this
    }
  }

  refreshState() {
    this.setState({
      isAuthed: localStorage.getItem('user') !== null,
      username: localStorage.getItem('user'),
      token: localStorage.getItem('token'),
      app: this
    });
  }

  login(info) {
    localStorage.setItem('user', info.username);
    localStorage.setItem('token', info.token);
    this.refreshState();
  }

  logout() {
    localStorage.clear();
    this.refreshState();
  }

  render() {
    return (
      <div>
        <div>
          <Header auth={this.state.isAuthed} />
          <main>
            <Switch>
              <Route exact path="/" component={Home}/>
              <UnAuthRoute path="/signin" auth={this.state.isAuthed} component={Signin} app={this} />
              <UnAuthRoute path="/signup" auth={this.state.isAuthed} component={Signup} />

              <AuthRoute path="/logout" auth={this.state.isAuthed} component={Logout} app={this}/>
              <AuthRoute path="/new" auth={this.state.isAuthed} component={Write}/>
              <AuthRoute path="/list" auth={this.state.isAuthed} component={List}/>
              <AuthRoute path="/view/:did" auth={this.state.isAuthed} component={View}/>
              <AuthRoute path="/inspect" auth={this.state.isAuthed} component={Inspect}/>
              <Route component={Error404} />
            </Switch>
        </main>
        </div>
      </div>
      );
  }
}

export default App;
