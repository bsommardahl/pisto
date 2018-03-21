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
  [@bs.send.pipe: t]
  external put : idObj('a) => Js.Promise.t(RevResponse.t) = "";
  [@bs.send.pipe: t]
  external remove : idObj('a) => Js.Promise.t(RevResponse.t) = "";
  [@bs.send.pipe: t] external post : 'a => Js.Promise.t(RevResponse.t) = "";
  [@bs.send.pipe: t] external get : string => Js.Promise.t(Js.t('a)) = "";
  [@bs.send.pipe: t]
  external find : FindRequest.queryT => Js.Promise.t(Js.t('a)) = "";
  [@bs.send] external closeConnection : t => Js.Promise.t(unit) = "close";
};

type t;

[@bs.module] [@bs.new]
external pouchdb : string => PouchDBConnection.t = "pouchdb";
/* db */
/* |> PouchDBConnection.put({"_id": "test"}) */
/* |> Js.Promise.then_(response => { */
/*      Js.log(response##ok); */
/*      Js.Promise.resolve(); */
/*    }); */
/* let query = FindRequest.query(~selector: {"test": "foo"}); */