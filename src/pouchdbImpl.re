type existing('existing) =
  {
    ..
    "_id": string,
    "_rev": string,
  } as 'existing;

type docArray('existing) = {. "docs": array('existing)};

[@bs.send]
external info :
  Pouchdb.t('existing, 'fresh) => Js.Promise.t(Pouchdb.DatabaseInfo.t) =
  "";

[@bs.send]
external destroy : Pouchdb.t('existing, 'fresh) => Js.Promise.t(unit) = "";

[@bs.send.pipe: Pouchdb.t('existing, 'fresh)]
external put : existing('existing) => Js.Promise.t(Pouchdb.RevResponse.t) =
  "";

[@bs.send.pipe: Pouchdb.t('existing, 'fresh)]
external remove : existing('existing) => Js.Promise.t(Pouchdb.RevResponse.t) =
  "";

[@bs.send.pipe: Pouchdb.t('existing, 'fresh)]
external post : 'fresh => Js.Promise.t(Pouchdb.RevResponse.t) = "";

[@bs.send.pipe: Pouchdb.t('existing, 'fresh)]
external get : string => Js.Promise.t('existing') = "";

[@bs.send.pipe: Pouchdb.t('existing, 'fresh)]
external find :
  Pouchdb.QueryBuilder.queryT => Js.Promise.t(docArray('existing)) =
  "";

[@bs.send]
external closeConnection : Pouchdb.t('existing, 'fresh) => Js.Promise.t(unit) =
  "close";

[@bs.send.pipe: Pouchdb.t('existing, 'fresh)]
external createIndex : 'a => unit = "";

type pouchdbFind;

type pouchdb;

[@bs.module "pouchdb-find"] external pouchdbFind : pouchdbFind = "default";

[@bs.send] external plugin : (pouchdb, pouchdbFind) => unit = "plugin";

[@bs.module "pouchdb"] external pouchdb : pouchdb = "default";

[@bs.module "pouchdb"] [@bs.new]
external init : string => Pouchdb.t('existing, 'fresh) = "default";

let connect = (dbNameOrUrl: string) => {
  plugin(pouchdb, pouchdbFind);
  let db = init(dbNameOrUrl);
  db;
};