import React from 'react';
import { Route, Link } from 'react-router-dom';

const NavLink = ({
  to,
  label,
  exact,
  activeClassName,
  className,
  isActive: getIsActive,
  "aria-current": ariaCurrent,
  ...rest
}) => {
  const path = typeof to === "object" ? to.pathname : to;

  // Regex taken from: https://github.com/pillarjs/path-to-regexp/blob/master/index.js#L202
  const escapedPath = path && path.replace(/([.+*?=^!:${}()[\]|/\\])/g, "\\$1");

  return (
    <Route
      path={escapedPath}
      exact={exact}
      children={({ location, match }) => {
        const isActive = !!(getIsActive ? getIsActive(match, location) : match);

        return (
          <li
            className={
              isActive
                ? [className, activeClassName].filter(i => i).join(" ")
                : className
            }
            aria-current={(isActive && ariaCurrent) || null}
            {...rest}
          >
          <Link to={to}>{label}</Link>
          </li>
        );
      }}
    />
  );
};

export default NavLink;
