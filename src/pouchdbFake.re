let info = (_db: Pouchdb.t('existing, 'fresh)) => {
  let dbInfo = {"db_name": "fakedb", "doc_count": 1, "update_seq": 2};
  Js.Promise.resolve(dbInfo);
};

let destroy = (_db: Pouchdb.t('existing, 'fresh)) => Js.Promise.resolve();

let put = (_item, _db: Pouchdb.t('existing, 'fresh)) =>
  Js.Promise.resolve({"ok": true, "id": "123", "rev": "3456"});

let remove = (_item, _db: Pouchdb.t('existing, 'fresh)) =>
  Js.Promise.resolve({"ok": true, "id": "123", "rev": "3456"});

let post = (_item, _db: Pouchdb.t('existing, 'fresh)) =>
  Js.Promise.resolve({"ok": true, "id": "123", "rev": "3456"});


type monkey = Pouchdb.existing({."_id": string, "_rev": string, "monkey": string});

let get = (_id: string, _db: Pouchdb.t('existing, 'fresh)) =>{
 let monkey: monkey = {"_id": "get", "_rev": "some rev", "monkey": "something"};
  Js.Promise.resolve(monkey);
};

type house = Pouchdb.existing({."_id": string, "_rev": string, "house": string});

let find = (_query, _db: Pouchdb.t('existing, 'fresh)) => {
  let sample: house = {"_id": "find", "_rev": "rev", "house": "tester"};
  let docArr: Pouchdb.docArray('existing) = {"docs": [|sample|]};
  Js.Promise.resolve(docArr);
};

let closeConnection = (_db: Pouchdb.t('existing, 'fresh)) =>
  Js.Promise.resolve();

/* let createIndex = (_index: 'a) => db; */
let connect = (_dbNameOrUrl: string) : Pouchdb.t('existing, 'fresh) => {
  info,
  destroy,
  put,
  get,
  post,
  /* find, */
  remove,
  closeConnection,
};