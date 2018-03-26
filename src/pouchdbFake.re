type existing('existing) =
  {
    ..
    "_id": string,
    "_rev": string,
  } as 'existing;

type docArray('existing) = {. "docs": array('existing)};

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

let get = (_id: string, _db: Pouchdb.t('existing, 'fresh)) =>
  Js.Promise.resolve(
    {"_id": "get", "_rev": "some rev", "name": "something"}:
                                                              existing(
                                                                'existing,
                                                              ),
  );

let find = (_query, _db: Pouchdb.t('existing, 'fresh)) => {
  let sample: existing('existing) = {
    "_id": "find",
    "_rev": "rev",
    "name": "something",
  };
  let docArr: docArray('existing) = {"docs": [|sample|]};
  Js.Promise.resolve(docArr);
};

let closeConnection = (_db: Pouchdb.t('existing, 'fresh)) =>
  Js.Promise.resolve();

/* let createIndex = (_index: 'a) => db; */
let connect = (_dbNameOrUrl: string) : Pouchdb.t('existing, 'fresh) => {
  "info": info,
  "destroy": destroy,
  "put": put,
  "get": get,
  "post": post,
  "find": find,
  "remove": remove,
  "closeConnection": closeConnection,
};