[@bs.send]
external info : Pouchdb.t => Js.Promise.t(Pouchdb.DatabaseInfo.t) = "";

[@bs.send] external destroy : Pouchdb.t => Js.Promise.t(unit) = "";

[@bs.send.pipe: Pouchdb.t]
external put : Js.t('a) => Js.Promise.t(Pouchdb.RevResponse.t) = "";

let put = item => {
  Js.log("pouchdb:: put");
  put(item);
};

[@bs.send.pipe: Pouchdb.t]
external remove : Js.t('a) => Js.Promise.t(Pouchdb.RevResponse.t) = "";

[@bs.send.pipe: Pouchdb.t]
external post : 'a => Js.Promise.t(Pouchdb.RevResponse.t) = "";

let post = item => {
  Js.log("pouchdb:: post");
  Js.log(item);
  post(item);
};

[@bs.send.pipe: Pouchdb.t] external get : string => Js.Promise.t('a) = "";

[@bs.send.pipe: Pouchdb.t]
external find : Pouchdb.QueryBuilder.queryT => Js.Promise.t('a) = "";

[@bs.send]
external closeConnection : Pouchdb.t => Js.Promise.t(unit) = "close";

[@bs.send.pipe: Pouchdb.t] external createIndex : 'a => unit = "";

type pouchdbFind;

type pouchdb;

[@bs.module "pouchdb-find"] external pouchdbFind : pouchdbFind = "default";

[@bs.send] external plugin : (pouchdb, pouchdbFind) => unit = "plugin";

[@bs.module "pouchdb"] external pouchdb : pouchdb = "default";

[@bs.module "pouchdb"] [@bs.new]
external init : string => Pouchdb.t = "default";

let connect = (dbNameOrUrl: string) => {
  plugin(pouchdb, pouchdbFind);
  let db = init(dbNameOrUrl);
  db;
};