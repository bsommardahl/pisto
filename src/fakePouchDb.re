open Pouchdb;

module FakeConnection: Pouchdb = {
  type t;
  let info = (db: Pouchdb.t) => {
    let dbInfo = {"db_name": "fakedb", "doc_count": 1, "update_seq": 2};
    Js.Promise.resolve(dbInfo);
  };
};

let getFake = () : PouchDBConnection.t => {
  let fake = FakeConnection;
  fake;
};
/*
 let destroy = (db: PouchDBConnection.t) => Js.Promise.resolve();
  let put = (item, db: PouchDBConnection.t) =>
    Js.Promise.resolve({"ok": true, "id": "123", "rev": "3456"});
  let remove = (item, db: PouchDBConnection.t) =>
    Js.Promise.resolve({"ok": true, "id": "123", "rev": "3456"});
  let post = (item, db: Pouchdb.t) =>
    Js.Promise.resolve({"ok": true, "id": "123", "rev": "3456"});
  let get = (id: string, db: PouchDBConnection.t) =>
    Js.Promise.resolve({"id": "get", "name": "something"});
  let find = (query, db: PouchDBConnection.t) =>
    Js.Promise.resolve({"docs": [{"id": "find", "name": "something"}]});
  let closeConnection = (db: PouchDBConnection.t) => Js.Promise.resolve();
  let createIndex = (index: 'a, db: PouchDBConnection.t) => db; */