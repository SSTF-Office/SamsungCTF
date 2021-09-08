import React from 'react';
import { Route, Redirect } from 'react-router-dom';

const AuthRoute = ({ component: Component, auth: Auth, ...rest }) => (
  <Route render={(props) => (
    Auth === true
      ? <Component {...rest} {...props} />
      : <Redirect to={{
          pathname: '/signin',
          state: { from: props.location }
        }} />
  )} />
)

const UnAuthRoute = ({ component: Component, auth: Auth, ...rest }) => (
  <Route render={(props) => (
    Auth === false
      ? <Component {...rest} {...props} />
      : <Redirect to={{
          pathname: '/',
          state: { from: props.location }
        }} />
  )} />
)

export {
  AuthRoute,
  UnAuthRoute,
};

