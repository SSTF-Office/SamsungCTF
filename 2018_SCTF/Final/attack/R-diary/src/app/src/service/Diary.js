import { post, get } from './api.js';

export function listDiaries() {
  const token = localStorage.getItem("token");
  return get('diaries', { headers: { 'x-access-token': token } });
}

export function getDiaries(id) {
  const token = localStorage.getItem("token");
  return get('diaries/' + id, { headers: { 'x-access-token': token } });
}

export function addDiaries(data) {
  const token = localStorage.getItem("token");
  return post('diaries', data, { headers: { 'x-access-token': token } });
}

export function inspectDiaries(uri, t) {
  const token = localStorage.getItem("token");
  return post('inspect/' + uri, {time: t}, { headers: { 'x-access-token': token } });
}
