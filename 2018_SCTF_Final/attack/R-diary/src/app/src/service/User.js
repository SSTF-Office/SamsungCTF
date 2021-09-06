import { post } from './api.js';

export function signin(uid, pw) {
  return post('signin', {
    userid: uid,
    password: pw
  });
}

export function signup(uid, pw) {
  return post('signup', {
    userid: uid,
    password: pw
  });
}
