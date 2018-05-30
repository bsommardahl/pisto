open Js.Promise;

type intent =
  | Viewing
  | Modifying(ExpenseType.t);

type state = {
  expenseTypes: list(ExpenseType.t),
  intent,
  showModal:bool
};

type action =
  | LoadExpenseTypes
  | ShowDialog
  | ExpenseTypesLoaded(list(ExpenseType.t))
  | RemoveExpenseType(ExpenseType.t)
  | ModifyExpenseType(ExpenseType.t)
  | CreateExpenseType(ExpenseType.New.t)
  | ExpenseTypeCreated(ExpenseType.t)
  | Change(intent);

let component = ReasonReact.reducerComponent("ExpenseTypeManagement");

let make = _children => {
  ...component,
  initialState: () => {expenseTypes: [], intent: Viewing,showModal:false},
  didMount: self => {
    self.send(LoadExpenseTypes);
    ReasonReact.NoUpdate;
  },
  reducer: (action, state) =>
    switch (action) {
    | LoadExpenseTypes =>
      ReasonReact.SideEffects(
        (
          self =>
            ExpenseTypeStore.getAll()
            |> Js.Promise.then_(expenseTypes => {
                 self.send(ExpenseTypesLoaded(expenseTypes));
                 Js.Promise.resolve();
               })
            |> ignore
        ),
      )
    | ExpenseTypesLoaded(expenseTypes) =>
      ReasonReact.Update({...state, expenseTypes})
    | Change(intent) => ReasonReact.Update({...state, intent})
    | ShowDialog => ReasonReact.Update({...state , showModal:!state.showModal})
    | RemoveExpenseType(prod) =>
      ReasonReact.UpdateWithSideEffects(
        {
          ...state,
          expenseTypes:
            state.expenseTypes
            |> List.filter((d: ExpenseType.t) => d.id !== prod.id),
        },
        (
          _self =>
            ExpenseTypeStore.remove(prod.id)
            |> then_((_) => resolve())
            |> ignore
        ),
      )
    | ModifyExpenseType(expenseType) =>
      ReasonReact.UpdateWithSideEffects(
        {
          showModal:false,
          intent: Viewing,
          expenseTypes:
            state.expenseTypes
            |> List.map((d: ExpenseType.t) =>
                 d.id === expenseType.id ? expenseType : d
               ),
        },
        (
          _self =>
            ExpenseTypeStore.update(expenseType)
            |> Js.Promise.catch(err => {
                 Js.log(err);
                 Js.Promise.resolve(expenseType);
               })
            |> ignore
        ),
      )
    | CreateExpenseType(prod) =>
      ReasonReact.SideEffects(
        (
          self =>
            ExpenseTypeStore.add(prod)
            |> Js.Promise.then_((newExpenseType: ExpenseType.t) => {
                 self.send(ExpenseTypeCreated(newExpenseType));
                 Js.Promise.resolve();
               })
            |> ignore
        ),
      )
    | ExpenseTypeCreated(prod) =>
      ReasonReact.Update({
        ...state,
        expenseTypes: List.concat([state.expenseTypes, [prod]]),
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
          (ReactUtils.sloc("admin.expenseTypes.header"))
        </div>
      </div>
      <div>
      <BsReactstrap.Modal 
        isOpen=(self.state.showModal)
        toggle = (self.state.showModal)
        className="Modal"
        >
            <BsReactstrap.ModalHeader toggle=(self.state.showModal)>
              "Delete Expense Type"
            </BsReactstrap.ModalHeader>
            <BsReactstrap.ModalBody>"Are you sure you want to delete this expense?"</BsReactstrap.ModalBody>
            <BsReactstrap.ModalFooter>
            /*The error says that prod doesn't exist whenever I try to delete it using RemoveExpenseType. */ 
                <Button 
                  local=true
                  className="remove-button-card"
                  label="action.delete"
                  onClick=((_)=>self.send(ShowDialog)/*RemoveExpenseType(prod))*/)/>
                <Button 
                  local=true
                  className="cancel-button-card"
                  label="action.cancelModal"
                  onClick=((_)=>self.send(ShowDialog))/>
            </BsReactstrap.ModalFooter>
        </BsReactstrap.Modal> 
      (
        switch (self.state.intent) {
        | Viewing =>
          <div className="expenseType-management">
            <table className="table">
              <thead>
                <tr>
                  <th />
                  <th> (ReactUtils.sloc("expenseType.name")) </th>
                 /* <th> (ReactUtils.sloc("expenseType.percent")) </th>*/
                  <th />
                </tr>
              </thead>
              <tbody>
                (
                  self.state.expenseTypes
                  |> List.map((prod: ExpenseType.t) =>
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
                           <Button
                             local=true
                             className="danger-card"
                             onClick=(
                               (_) => self.send(ShowDialog)
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
            <ExpenseTypeEdit
              expenseTypes=self.state.expenseTypes
              onSubmit=(
                ({values}) =>
                  self.send(CreateExpenseType({name: values.name}))
              )
            />
          </div>
        | Modifying(expenseType) =>
          <div>
            <h3> (ReactUtils.sloc("action.edit")) </h3>
            <ExpenseTypeEdit
              expenseTypes=self.state.expenseTypes
              expenseType=(Some(expenseType))
              onSubmit=(
                ({values}) =>
                  self.send(
                    ModifyExpenseType({
                      id: expenseType.id,
                      name: values.name,
                    }),
                  )
              )
            />
          </div>
        }
      )</div>
    </div>;
  },
};