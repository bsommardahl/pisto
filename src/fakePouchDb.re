type t;

type idObj('a) = {.. "_id": string} as 'a;

let info = (_db: t) => {
  let dbInfo = {"db_name": "fakedb", "doc_count": 1, "update_seq": 2};
  Js.Promise.resolve(dbInfo);
};

let destroy = (_db: t) => Js.Promise.resolve();

let put = (_item, _db: t) =>
  Js.Promise.resolve({"ok": true, "id": "123", "rev": "3456"});

let remove = (_item, _db: t) =>
  Js.Promise.resolve({"ok": true, "id": "123", "rev": "3456"});

let post = (_item, _db: t) =>
  Js.Promise.resolve({"ok": true, "id": "123", "rev": "3456"});

let get = (_id: string, _db: t) =>
  Js.Promise.resolve({"id": "get", "name": "something"});

let find = (_query, _db: t) =>
  Js.Promise.resolve({"docs": [{"id": "find", "name": "something"}]});

let closeConnection = (_db: t) => Js.Promise.resolve();

let createIndex = (_index: 'a, db: t) => db;