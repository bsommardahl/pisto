open Js.Promise;

type intent =
  | Viewing
  | Modifying(Discount.t);

type state = {
  discounts: list(Discount.t),
  intent,
};

type action =
  | LoadDiscounts
  | DiscountsLoaded(list(Discount.t))
  | RemoveDiscount(Discount.t)
  | ModifyDiscount(Discount.t)
  | CreateDiscount(Discount.NewDiscount.t)
  | DiscountCreated(Discount.t)
  | Change(intent);

let component = ReasonReact.reducerComponent("DiscountManagement");

let make = _children => {
  ...component,
  initialState: () => {discounts: [], intent: Viewing},
  didMount: self => {
    self.send(LoadDiscounts);
    ReasonReact.NoUpdate;
  },
  reducer: (action, state) =>
    switch (action) {
    | LoadDiscounts =>
      ReasonReact.SideEffects(
        (
          self =>
            DiscountStore.getAll()
            |> Js.Promise.then_(discounts => {
                 self.send(DiscountsLoaded(discounts));
                 Js.Promise.resolve();
               })
            |> ignore
        ),
      )
    | DiscountsLoaded(discounts) => ReasonReact.Update({...state, discounts})
    | Change(intent) => ReasonReact.Update({...state, intent})
    | RemoveDiscount(prod) =>
      ReasonReact.UpdateWithSideEffects(
        {
          ...state,
          discounts:
            state.discounts
            |> List.filter((d: Discount.t) => d.id !== prod.id),
        },
        (
          _self =>
            DiscountStore.remove(prod.id)
            |> then_((_) => resolve())
            |> ignore
        ),
      )
    | ModifyDiscount(discount) =>
      ReasonReact.UpdateWithSideEffects(
        {
          intent: Viewing,
          discounts:
            state.discounts
            |> List.map((d: Discount.t) =>
                 d.id === discount.id ? discount : d
               ),
        },
        (
          _self =>
            DiscountStore.update(discount)
            |> Js.Promise.catch(err => {
                 Js.log(err);
                 Js.Promise.resolve(discount);
               })
            |> ignore
        ),
      )
    | CreateDiscount(prod) =>
      ReasonReact.SideEffects(
        (
          self =>
            DiscountStore.add(prod)
            |> Js.Promise.then_((newDiscount: Discount.t) => {
                 self.send(DiscountCreated(newDiscount));
                 Js.Promise.resolve();
               })
            |> ignore
        ),
      )
    | DiscountCreated(prod) =>
      ReasonReact.Update({
        ...state,
        discounts: List.concat([state.discounts, [prod]]),
      })
    },
  render: self => {
    let goBack = (_) => ReasonReact.Router.push("/admin");
    <div className="admin-menu">
      <div className="header">
        <div className="header-menu">
          <div className="card wide-card quiet-card" onClick=goBack>
            (ReactUtils.s("Atras"))
          </div>
        </div>
        <div className="header-options">
          (ReactUtils.sloc("admin.discounts.header"))
        </div>
      </div>
      (
        switch (self.state.intent) {
        | Viewing =>
          <div className="discount-management">
            <table className="table">
              <thead>
                <tr>
                  <th />
                  <th> (ReactUtils.sloc("discount.name")) </th>
                  <th> (ReactUtils.sloc("discount.percent")) </th>
                  <th />
                </tr>
              </thead>
              <tbody>
                (
                  self.state.discounts
                  |> List.map((prod: Discount.t) =>
                       <tr key=prod.id>
                         <td>
                           <Button
                             local=true
                             disabled=false
                             onClick=(
                               (_) => self.send(Change(Modifying(prod)))
                             )
                             label="action.edit"
                           />
                         </td>
                         <td> (ReactUtils.s(prod.name)) </td>
                         <td>
                           (ReactUtils.s(prod.percent |> Percent.toDisplay))
                         </td>
                         <td>
                           <Button
                             local=true
                             className="danger-card"
                             onClick=(
                               (_) => self.send(RemoveDiscount(prod))
                             )
                             label="action.delete"
                           />
                         </td>
                       </tr>
                     )
                  |> Array.of_list
                  |> ReasonReact.arrayToElement
                )
              </tbody>
            </table>
            <h3> (ReactUtils.sloc("action.create")) </h3>
            <DiscountEdit
              discounts=self.state.discounts
              onSubmit=(
                ({values}) =>
                  self.send(
                    CreateDiscount({
                      percent: values.percent |> Percent.toT,
                      name: values.name,
                    }),
                  )
              )
            />
          </div>
        | Modifying(discount) =>
          <div>
            <h3> (ReactUtils.sloc("action.edit")) </h3>
            <DiscountEdit
              discounts=self.state.discounts
              discount=(Some(discount))
              onSubmit=(
                ({values}) =>
                  self.send(
                    ModifyDiscount({
                      id: discount.id,
                      name: values.name,
                      percent: values.percent |> Percent.toT,
                    }),
                  )
              )
            />
          </div>
        }
      )
    </div>;
  },
};