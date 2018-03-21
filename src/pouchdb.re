module DatabaseInfo = {
  type t = {
    .
    "db_name": string,
    "doc_count": int,
    "update_seq": int,
  };
};

module GetResponse = {
  type t = {
    .
    "_id": string,
    "_rev": string,
  };
  /* and more props depending on the type */
};

module RevResponse = {
  type t = {
    .
    "ok": bool,
    "id": string,
    "rev": string,
  };
};

module FindResponse = {
  type t;
  [@bs.get] external docs : t => array('a) = "";
  /* Not sure this works because we don't know what 'a is. */
};

module FindRequest = {
  type t;
  type queryT;
  [@bs.obj]
  external query :
    (
      ~selector: Js.t({..}),
      ~fields: option(array(string))=?,
      ~sort: option(array(string))=?
    ) =>
    queryT =
    "";
  [@bs.val] external selector : t => Js.Json.t = "";
  [@bs.val] external fields : t => Js.Array.t(Js.Json.t) = "";
  [@bs.val] external sort : t => Js.Array.t(Js.Json.t) = "";
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
  [@bs.send.pipe: t]
  external get : string => Js.Promise.t(GetResponse.t) = "";
  [@bs.send.pipe: t]
  external find : FindRequest.queryT => Js.Promise.t(FindResponse.t) = "";
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