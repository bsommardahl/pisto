module type Database = {
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
  module Query: {
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
  type t;
  type idObj('a) = {.. "_id": string} as 'a;
  let info: t => Js.Promise.t(DatabaseInfo.t);
  let destroy: t => Js.Promise.t(unit);
  let put: (idObj('a), t) => Js.Promise.t(RevResponse.t);
  let post: ('a, t) => Js.Promise.t(RevResponse.t);
  let get: (string, t) => Js.Promise.t(Js.t('a));
  let find: (Query.queryT, t) => Js.Promise.t(Js.t('a));
  let closeConnection: t => Js.Promise.t(unit);
  let createIndex: ('a, t) => unit;
  let connect: string => t;
};