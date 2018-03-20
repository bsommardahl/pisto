module DatabaseInfo = {
  type t = {
    .
    "db_name": string,
    "doc_count": int,
    "update_seq": int
  };
};

module GetResponse = {
  type t;
  [@bs.get] external id : t => string = "_id";
  [@bs.get] external rev : t => string = "_rev";
  /* and more props depending on the type */
};

module PutResponse = {
  type t = {
    .
    "ok": bool,
    "id": string,
    "rev": string
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
  [@bs.send.pipe : t]
  external put : idObj('a) => Js.Promise.t(PutResponse.t) = "";
  [@bs.send.pipe : t]
  external get : string => Js.Promise.t(GetResponse.t) = "";
  [@bs.send.pipe : t]
  external find : FindRequest.queryT => Js.Promise.t(FindResponse.t) = "";
};

type t;

[@bs.module "pouchdb"] [@bs.new]
external connect : string => PouchDBConnection.t = "default";

let db = connect("orders");

/* db */
/* |> PouchDBConnection.info */
/* |> Js.Promise.then_(info => { */
/*      Js.log(info##db_name); */
/*      Js.Promise.resolve(); */
/*    }); */
/* db */
/* |> PouchDBConnection.put({"_id": "test"}) */
/* |> Js.Promise.then_(response => { */
/*      Js.log(response##ok); */
/*      Js.Promise.resolve(); */
/*    }); */

let query = FindRequest.query(~selector: {"test": "foo"});
