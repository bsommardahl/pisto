module DatabaseInfo: {
  type t = {
    .
    "db_name": string,
    "doc_count": int,
    "update_seq": int,
  };
};

module RevResponse: {
  type t = {
    .
    "ok": bool,
    "id": string,
    "rev": string,
  };
};

module QueryBuilder: {
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

type existing('existing) =
  {
    ..
    "_id": string,
    "_rev": string,
  } as 'existing;

type docArray('existing) = {. "docs": array('existing)};

type t('existing, 'fresh) = {
  .
  "info": unit => Js.Promise.t(DatabaseInfo.t),
  "destroy": unit => Js.Promise.t(unit),
  "put": existing('existing) => Js.Promise.t(RevResponse.t),
  "get": string => Js.Promise.t('existing),
  "post": 'fresh => Js.Promise.t(RevResponse.t),
  "find": QueryBuilder.queryT => Js.Promise.t(docArray('existing)),
  "remove": 'existing => Js.Promise.t(RevResponse.t),
  "closeConnection": unit => Js.Promise.t(unit),
};