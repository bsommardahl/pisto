module DatabaseInfo = {
  type t = {
    .
    "db_name": string,
    "doc_count": int,
    "update_seq": int,
  };
};

module RevResponse = {
  type t = {
    .
    "ok": bool,
    "id": string,
    "rev": string,
  };
};

module FindRequest = {
  type t;
  type queryT;
  [@bs.obj]
  external query :
    (
      ~selector: Js.t({..}),
      ~fields: option(array(string))=?,
      ~sort: option(array(string))=?,
      unit
    ) =>
    queryT =
    "";
};

module PouchDBConnection = {
  type t;
  type idObj('a) = {.. "_id": string} as 'a;
  [@bs.send] external info : t => Js.Promise.t(DatabaseInfo.t) = "";
  [@bs.send] external destroy : t => Js.Promise.t(unit) = "";
  [@bs.send.pipe: t]
  external put : idObj('a) => Js.Promise.t(RevResponse.t) = "";
  [@bs.send.pipe: t]
  external remove : idObj('a) => Js.Promise.t(RevResponse.t) = "";
  [@bs.send.pipe: t] external post : 'a => Js.Promise.t(RevResponse.t) = "";
  [@bs.send.pipe: t] external get : string => Js.Promise.t(Js.t('a)) = "";
  [@bs.send.pipe: t]
  external find : FindRequest.queryT => Js.Promise.t(Js.t('a)) = "";
  [@bs.send] external closeConnection : t => Js.Promise.t(unit) = "close";
  [@bs.send.pipe: t] external createIndex : 'a => unit = "";

};

type t;

type findPlugin;

[@bs.module] external getFindPlugin : findPlugin = "pouchdb-find";

[@bs.module "pouchdb"] external plugin : findPlugin => unit = "";

[@bs.module] [@bs.new]
external pouchdb : string => PouchDBConnection.t = "pouchdb";

let pouchdb = (dbNameOrUrl: string) => {
  plugin(getFindPlugin);
  let db = pouchdb(dbNameOrUrl);  
  db;
};