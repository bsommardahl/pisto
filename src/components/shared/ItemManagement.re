module Create = (Store: DbStore.Interface) => {
  type intent =
    | Viewing
    | Modifying(Store.item)
    | Deleting(Store.item);

  type state = {
    items: list(Store.item),
    showItemDialog: bool,
    showEditItemDialog: bool,
    intent,
  };

  type action =
    | LoadItems
    | ItemsLoaded(list(Store.item))
    | ShowDialog(Store.item)
    | HideDialog
    | ShowItemDialog
    | HideItemDialog
    | ShowEditItemDialog
    | HideEditItemDialog
    | RemoveItem(Store.item)
    | ItemRemoved(Store.item)
    | ModifyItem(Store.item)
    | ItemModified(Store.item)
    | CreateItem(Store.newItem)
    | ItemCreated(Store.item)
    | Change(intent);

  let component = ReasonReact.reducerComponent("ItemManagement");

  let goBack = _ => ReasonReact.Router.push("/admin");

  let make = (~renderCreate, ~renderEdit, ~renderItem, _children) => {
    ...component,
    initialState: () => {
      items: [],
      intent: Viewing,
      showItemDialog: false,
      showEditItemDialog: false,
    },
    didMount: self => self.send(LoadItems),
    reducer: (action, state) =>
      switch (action) {
      | LoadItems =>
        ReasonReact.SideEffects(
          (
            self =>
              Store.getAll()
              |> Js.Promise.then_(items => {
                   self.send(ItemsLoaded(items));
                   Js.Promise.resolve();
                 })
              |> ignore
          ),
        )
      | ShowItemDialog => ReasonReact.Update({...state, showItemDialog: true})
      | HideItemDialog =>
        ReasonReact.Update({...state, showItemDialog: false, intent: Viewing})
      | ShowEditItemDialog =>
        ReasonReact.Update({...state, showEditItemDialog: true})
      | HideEditItemDialog =>
        ReasonReact.Update({
          ...state,
          showEditItemDialog: false,
          intent: Viewing,
        })
      | ShowDialog(item) =>
        ReasonReact.Update({...state, intent: Deleting(item)})
      | HideDialog => ReasonReact.Update({...state, intent: Viewing})
      | ItemsLoaded(items) => ReasonReact.Update({...state, items})
      | Change(intent) =>
        ReasonReact.Update({...state, intent, showEditItemDialog: true})
      | RemoveItem(item) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, intent: Viewing},
          (
            self =>
              Store.remove(~id=Store.id(item))
              |> Js.Promise.then_(() => {
                   self.send(ItemRemoved(item));
                   Js.Promise.resolve();
                 })
              |> ignore
          ),
        )
      | ItemRemoved(item) =>
        ReasonReact.Update({
          ...state,
          items:
            state.items |> List.filter(i => Store.id(i) !== Store.id(item)),
        })
      | ModifyItem(item) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, intent: Viewing},
          (
            self =>
              Store.update(item)
              |> Js.Promise.then_(item => {
                   self.send(ItemModified(item));
                   Js.Promise.resolve();
                 })
              |> Js.Promise.catch(err => {
                   Js.log(err);
                   Js.Promise.resolve();
                 })
              |> ignore
          ),
        )
      | ItemModified(item) =>
        ReasonReact.Update({
          ...state,
          items:
            state.items
            |> List.map(i => Store.id(i) === Store.id(item) ? item : i),
        })
      | CreateItem(item) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, showItemDialog: false},
          (
            self =>
              Store.add(item)
              |> Js.Promise.then_(item => {
                   self.send(ItemCreated(item));
                   Js.Promise.resolve();
                 })
              |> ignore
          ),
        )
      | ItemCreated(item) =>
        ReasonReact.Update({...state, items: state.items @ [item]})
      },
    render: self =>
      <div className="admin-menu">
        <div className="header">
          <div className="header-menu">
            <div
              className="card wide-card pay-button-card"
              onClick=(_ => self.send(ShowItemDialog))>
              (ReactUtils.s("Create"))
            </div>
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
                    self.state.items
                    |> List.map(i =>
                         renderItem(
                           ~item=i,
                           ~onEditClick=
                             _ => self.send(Change(Modifying(i))),
                           ~onDeleteClick=_ => self.send(ShowDialog(i)),
                         )
                       )
                    |> Array.of_list
                    |> ReasonReact.array
                  )
                </tbody>
              </table>
              <ItemModal
                isOpen=self.state.showItemDialog
                label="action.createDiscount"
                onClose=(_ => self.send(HideItemDialog))
                render=(
                  () =>
                    renderCreate(
                      ~items=self.state.items,
                      ~onSubmit=item => self.send(CreateItem(item)),
                      ~onCancel=_ => self.send(HideItemDialog),
                    )
                )
              />
            </div>
          | Modifying(item) =>
            <div>
              <ItemModal
                isOpen=self.state.showEditItemDialog
                label="action.editProduct"
                onClose=(_ => self.send(HideEditItemDialog))
                render=(
                  () =>
                    renderEdit(
                      ~items=self.state.items,
                      ~item,
                      ~onSubmit=item => self.send(ModifyItem(item)),
                      ~onCancel=_ => self.send(HideEditItemDialog),
                    )
                )
              />
            </div>
          | Deleting(item) =>
            <DeleteModal
              contentLabel="modal.deleteDiscountContent"
              label="modal.deleteDiscount"
              isOpen=true
              onConfirm=(() => self.send(RemoveItem(item)))
              onCancel=(() => self.send(HideDialog))
            />
          }
        )
      </div>,
  };
};