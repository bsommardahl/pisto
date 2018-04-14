

open Js.Promise;

type state = {discounts: list(Discount.t)};

type action =
  | LoadDiscounts(list(Discount.t))
  | DiscountRemoved(Discount.t)
  | DiscountModified(Discount.t)
  | NewDiscountCreated(Discount.t);

let component = ReasonReact.reducerComponent("DiscountManagement");

let make = _children => {
  ...component,
  didMount: self => {
    DiscountStore.getAll()
    |> Js.Promise.then_(discounts => {
         self.send(LoadDiscounts(discounts));
         Js.Promise.resolve();
       })
    |> ignore;
    ReasonReact.NoUpdate;
  },
  initialState: () => {discounts: []},
  reducer: (action, state) =>
    switch (action) {
    | LoadDiscounts(discounts) => ReasonReact.Update({discounts: discounts})
    | DiscountRemoved(dis) =>
      ReasonReact.Update({
        discounts:
          state.discounts |> List.filter((d: Discount.t) => d.id !== dis.id),
      })
    | DiscountModified(dis) =>
      ReasonReact.Update({
        discounts:
          state.discounts
          |> List.map((d: Discount.t) => d.id !== dis.id ? dis : d),
      })
    | NewDiscountCreated(dis) =>
      ReasonReact.Update({discounts: List.concat([state.discounts, [dis]])})
    },
  render: self => {
    let goBack = (_) => ReasonReact.Router.push("/admin");
    let removeDiscount = (p: Discount.t) => {
      DiscountStore.remove(p.id)
      |> then_((_) => {
           self.send(DiscountRemoved(p));
           resolve();
         })
      |> ignore;
      ();
    };
    let modifyDiscount = (modifiedDiscount: Discount.t) =>
      DiscountStore.update(modifiedDiscount)
      |> then_((_) => {
           self.send(DiscountModified(modifiedDiscount));
           resolve();
         })
      |> ignore;
    let createDiscount = (newDiscount: Discount.NewDiscount.t) => {
      DiscountStore.add(newDiscount)
      |> Js.Promise.then_((newDiscount: Discount.t) => {
           self.send(NewDiscountCreated(newDiscount));
           Js.Promise.resolve();
         })
      |> ignore;
      ();
    };
    <div className="admin-menu">
      <div className="header">
        <div className="header-menu">
          <div className="card wide-card quiet-card" onClick=goBack>
            (ReactUtils.s("Atras"))
          </div>
        </div>
        <div className="header-options"> (ReactUtils.s("Gestion de Descuentos")) </div>
      </div>
      <div className="discount-management">
        <table className="table">
          <thead>
            <tr>
              <th />
              <th> (ReactUtils.s("Nombre")) </th>
              <th> (ReactUtils.s("Percent")) </th>
            </tr>
          </thead>
          <tbody>
            (
              self.state.discounts
              |> List.map((d: Discount.t) =>
                   <DiscountManagementRow
                     discount=d
                     remove=removeDiscount
                     modify=modifyDiscount
                     key=d.id
                   />
                 )
              |> Array.of_list
              |> ReasonReact.arrayToElement
            )
          </tbody>
          <tfoot> <DiscountManagementNew create=createDiscount /> </tfoot>
        </table>
      </div>
    </div>;
  },
};