type existing('existing) =
  {
    ..
    "_id": string,
    "_rev": string,
  } as 'existing;

type docArray('existing) = {. "docs": array('existing)};

let info = () => {
  let dbInfo = {"db_name": "fakedb", "doc_count": 1, "update_seq": 2};
  Js.Promise.resolve(dbInfo);
};

let destroy = () => Js.Promise.resolve();

let put = _item =>
  Js.Promise.resolve({"ok": true, "id": "123", "rev": "3456"});

let remove = _item =>
  Js.Promise.resolve({"ok": true, "id": "123", "rev": "3456"});

let post = _item =>
  Js.Promise.resolve({"ok": true, "id": "123", "rev": "3456"});

let get = (_id: string) =>
  Js.Promise.resolve(
    {"_id": "get", "_rev": "some rev", "name": "something"}:
                                                              existing(
                                                                'existing,
                                                              ),
  );

let find = _query => {
  let sample: existing('existing) = {
    "_id": "find",
    "_rev": "rev",
    "name": "something",
  };
  let docArr: docArray('existing) = {"docs": [|sample|]};
  Js.Promise.resolve(docArr);
};

let closeConnection = () => Js.Promise.resolve();

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