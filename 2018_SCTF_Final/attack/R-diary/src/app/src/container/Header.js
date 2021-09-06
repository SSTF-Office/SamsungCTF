import React from 'react';
import { Link } from 'react-router-dom';

import NavLink from '../component/NavLink';

const UnAuthLinks = () => (
  <ul className="right">
  <NavLink exact to="/signin" label="Sign In" activeClassName="active" />
  <NavLink exact to="/signup" label="Sign Up" activeClassName="active" />
  </ul>
)

const AuthLinks = () => (
  <ul className="right">
  <NavLink exact to="/inspect" label="Inspect Diary" activeClassName="active" />
  <NavLink exact to="/new" label="New Diary" activeClassName="active" />
  <NavLink exact to="/list" label="List Diary" activeClassName="active" />
  <NavLink exact to="/logout" label="Sign Out" activeClassName="active" />
  </ul>
)

const Header = ({auth: Auth}) => {
  if (Auth) {
    return (
      <nav>
      <div className="nav-wrapper">
      <Link to="/" className="brand-logo center"><img id="logo" alt="#" src="/static/img/diary.png"/></Link>
      <AuthLinks />
      </div>
      </nav>
    )
  } else {
    return (
      <nav>
      <div className="nav-wrapper">
      <Link to="/" className="brand-logo center"><img id="logo" alt="#" src="/static/img/diary.png"/></Link>
      <UnAuthLinks />
      </div>
      </nav>
    )}
}

export default Header;
