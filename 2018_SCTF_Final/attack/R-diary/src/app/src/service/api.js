import axios from 'axios';

const apiServer = "http://localhost:3001/";

export function post(point, data, rest) {
  return axios.post(apiServer + point, data, rest);
}

export function get(point, rest) {
  return axios.get(apiServer + point, rest);
}
