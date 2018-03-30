open Js.Promise;

open PouchdbImpl;

let db = PouchdbImpl.connect("discounts", Config.Database.livePouchDbConfig);

let add = (newDiscount: Discount.NewDiscount.t) =>
  db
  |> post(newDiscount |> Discount.NewDiscount.mapToJs)
  |> then_(revResponse =>
       db
       |> get(revResponse##id)
       |> then_(js => {
            let o = js |> Discount.mapFromJs;
            Js.log(
              "DiscountStore:: added new Discount for " ++ newDiscount.name,
            );
            resolve(o);
          })
     );

let getAll = () =>
  db
  |> find(
       Pouchdb.QueryBuilder.query(~selector={
                                    "_id": {
                                      "$gt": Js.null,
                                    },
                                  }, ()),
     )
  |> then_(response => resolve(response##docs))
  |> then_(docs => {
       let mapped: array(Discount.t) =
         docs |> Array.map(d => Discount.mapFromJs(d));
       Js.log(
         "DiscountStore:: got Discounts: "
         ++ string_of_int(docs |> Array.length),
       );
       resolve(mapped);
     })
  |> then_(discounts => resolve(Array.to_list(discounts)));

let update = (discount: Discount.t) : Js.Promise.t(Discount.t) =>
  db
  |> get(discount.id)
  |> then_(js => {
       let modified = discount |> Discount.mapToJsWithRev(js##_id, js##_rev);
       db
       |> put(modified)
       |> then_(_rev => {
            Js.log("DiscountStore:: updated Discount for " ++ js##name);
            resolve(Discount.mapFromJs(js));
          });
     });

let remove = (discoundId: string) : t(unit) =>
  db
  |> PouchdbImpl.get(discoundId)
  |> then_(item =>
       db
       |> remove(item)
       |> then_((_) => {
            Js.log(
              "DiscountStore:: removed Discount with id: " ++ discoundId,
            );
            resolve();
          })
     );