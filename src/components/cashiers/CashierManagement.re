open Js.Promise;

type intent =
  | Viewing
  | Modifying(Cashier.t);

type state = {
  cashiers: list(Cashier.t),
  intent,
};

type action =
  | LoadCashiers(list(Cashier.t))
  | CashierRemoved(Cashier.t)
  | ModifyCashier(Cashier.t)
  | NewCashierCreated(Cashier.t)
  | Change(intent);

let component = ReasonReact.reducerComponent("CashierManagement");

let make = _children => {
  ...component,
  didMount: self => {
    CashierStore.getAll()
    |> Js.Promise.then_(cashiers => {
         self.send(LoadCashiers(cashiers));
         Js.Promise.resolve();
       })
    |> ignore;
    ReasonReact.NoUpdate;
  },
  initialState: () => {cashiers: [], intent: Viewing},
  reducer: (action, state) =>
    switch (action) {
    | LoadCashiers(cashiers) => ReasonReact.Update({...state, cashiers})
    | Change(intent) => ReasonReact.Update({...state, intent})
    | CashierRemoved(dis) =>
      ReasonReact.Update({
        ...state,
        cashiers:
          state.cashiers |> List.filter((d: Cashier.t) => d.id !== dis.id),
      })
    | ModifyCashier(cashier) => 
      ReasonReact.UpdateWithSideEffects({
        intent: Viewing,
        cashiers:
          state.cashiers
          |> List.map((d: Cashier.t) => d.id === cashier.id ? cashier : d),
      }, _ => {
        CashierStore.update(cashier)
        |> ignore;
    
      })
    | NewCashierCreated(dis) =>
      ReasonReact.Update({
        ...state,
        cashiers: List.concat([state.cashiers, [dis]]),
      })
    },
  render: self => {
    let goBack = (_) => ReasonReact.Router.push("/admin");
    let isUnique = (originalPin, pin)=> { 
      if(originalPin===pin){
        None;
      }
      else{
        let duplicates = self.state.cashiers 
        |> List.filter((c:Cashier.t)=> c.pin===pin);
        let isDuplicate=(duplicates |> List.length) > 0;
        
        isDuplicate
        ? Some("validation.duplicate") 
        : None;
      };
    };
    let removeCashier = (p: Cashier.t) => {
      CashierStore.remove(p.id)
      |> then_((_) => {
           self.send(CashierRemoved(p));
           resolve();
         })
      |> ignore;
      ();
    };
    let createCashier = (name: string, pin: string) => {
      CashierStore.add({name, pin})
      |> Js.Promise.then_((newCashier: Cashier.t) => {
           self.send(NewCashierCreated(newCashier));
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
        <div className="header-options">
          (ReactUtils.sloc("admin.cashiers.header"))
        </div>
      </div>
      (
        switch (self.state.intent) {
        | Viewing =>
          <div className="cashier-management">
            <table className="table">
              <thead>
                <tr>
                  <th />
                  <th> (ReactUtils.s("Nombre")) </th>
                  <th> (ReactUtils.s("Pin")) </th>
                </tr>
              </thead>
              <tbody>
                (
                  self.state.cashiers
                  |> List.map((d: Cashier.t) =>
                       <tr key=d.id>
                         <td>
                           <Button
                             local=true
                             onClick=(
                               (_) => self.send(Change(Modifying(d)))
                             )
                             label="action.edit"
                           />
                         </td>
                         <td> (ReactUtils.s(d.name)) </td>
                         <td> (ReactUtils.s(d.pin)) </td>
                         <td>
                           <Button
                             local=true
                             onClick=((_) => removeCashier(d))
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
            <CashierEdit
              isUnique
              onSubmit=(
                ({values}) => createCashier(values.name, values.pin)
              )
            />
          </div>
        | Modifying(cashier) =>
          <div>
            <h3> (ReactUtils.sloc("action.edit")) </h3>
            <CashierEdit
              isUnique
              name=cashier.name
              pin=cashier.pin
              onSubmit=(
                ({values}) =>
                  self.send(ModifyCashier({
                    id:cashier.id,
                    name: values.name, 
                    pin: values.pin
                  }))
              )
            />
          </div>
        }
      )
    </div>;
  },
};