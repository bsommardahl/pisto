open Jest;

open ExpectJs;

open PouchdbImpl;

let dbName = "testdb";

let testDbConfig: Config.Database.pouchDbConfig = {
  local: {
    host: "",
    options: {
      "auth": {
        "username": "",
        "password": "",
      },
      "auto_compaction": true,
    },
  },
  remote: None,
};

let db = PouchdbImpl.connect(dbName, testDbConfig);

describe("The PouchDb Wrapper", () => {
  describe("when getting database info", () =>
    testAsync("it should return the database name", finish => {
      db
      |> info
      |> Js.Promise.then_(info => {
           let assertion = expect(info##db_name) |> toEqual("testdb");
           finish(assertion);
           Js.Promise.resolve();
         })
      |> ignore;
      db |> closeConnection |> ignore;
    })
  );
  describe("when posting an item into the database", () =>
    testAsync("it should create the record and return a new id", finish => {
      db
      |> post({"name": "byron"})
      |> Js.Promise.then_(response => {
           expect(response##ok) |> toBe(true) |> ignore;
           finish(
             expect(response##id |> String.length) |> toBeGreaterThan(30),
           );
           Js.Promise.resolve();
         })
      |> ignore;
      db |> closeConnection |> ignore;
    })
  );
  describe("when upserting a new item in the database", () =>
    testAsync("it should modify the record and return a new rev", finish => {
      let id = "upsert_new_test" ++ string_of_float(Js.Date.now());
      db
      |> put({"_id": id, "_rev": "some rev", "name": "byron"})
      |> Js.Promise.then_(response => {
           expect(response##ok) |> toBe(true) |> ignore;
           finish(expect(response##id) |> toEqual(id));
           Js.Promise.resolve();
         })
      |> ignore;
      db |> closeConnection |> ignore;
    })
  );
  describe("when upserting an existing item in the database", () =>
    testAsync("it should modify the record and return a new rev", finish => {
      let name = "upsert_existing_test" ++ string_of_float(Js.Date.now());
      let newOrder = {"name": name};
      db
      |> post(newOrder)
      |> Js.Promise.then_(response => {
           let rev = response##rev;
           let id = response##id;
           let modification = {"_id": id, "name": "george", "_rev": rev};
           db
           |> put(modification)
           |> Js.Promise.then_(response => {
                finish(expect(response##rev) |> not_ |> toEqual(rev));
                db |> closeConnection |> ignore;
                Js.Promise.resolve();
              })
           |> ignore;
           Js.Promise.resolve();
         })
      |> ignore;
      ();
    })
  );
  describe("when deleting an existing item from the database", () =>
    testAsync("it should mark the item as deleted return a new rev", finish => {
      db
      |> PouchdbImpl.post({"name": "byron"})
      |> Js.Promise.then_(created => {
           db
           |> PouchdbImpl.remove({"_id": created##id, "_rev": created##rev})
           |> Js.Promise.then_(removed => {
                finish(
                  expect(removed##rev) |> not_ |> toEqual(created##rev),
                );
                db |> PouchdbImpl.closeConnection |> ignore;
                Js.Promise.resolve();
              })
           |> ignore;
           Js.Promise.resolve();
         })
      |> ignore;
      ();
    })
  );
  describe("when fetching an existing item from the database", () =>
    testAsync("it should retrieve the correct item", finish => {
      db
      |> PouchdbImpl.post({"name": "byron"})
      |> Js.Promise.then_(created => {
           db
           |> PouchdbImpl.get(created##id)
           |> Js.Promise.then_(fetched => {
                finish(expect(fetched##name) |> toEqual("byron"));
                db |> PouchdbImpl.closeConnection |> ignore;
                Js.Promise.resolve();
              })
           |> ignore;
           Js.Promise.resolve();
         })
      |> ignore;
      ();
    })
  );
  describe("when querying for existing items in the database", () =>
    testAsync("it should retrieve the correct items", finish => {
      let name = string_of_float(Js.Date.now());
      db
      |> post({"name": name})
      |> Js.Promise.then_(created => {
           expect(created##ok) |> toBeTruthy |> ignore;
           let rev = created##rev;
           db
           |> PouchdbImpl.find(
                Pouchdb.QueryBuilder.query(~selector={"name": name}, ()),
              )
           |> Js.Promise.then_(response => {
                let docs = response##docs;
                expect(docs[0]##_rev) |> toEqual(rev) |> ignore;
                finish(expect(docs[0]##name) |> toEqual(name));
                db |> PouchdbImpl.closeConnection |> ignore;
                Js.Promise.resolve();
              })
           |> Js.Promise.catch(err => Js.Promise.resolve(Js.log(err)))
           |> ignore;
           Js.Promise.resolve();
         })
      |> ignore;
      ();
    })
  );
});