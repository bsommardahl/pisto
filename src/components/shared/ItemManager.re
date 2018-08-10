module Create = (Store: DbStore.Interface) => {
  type intent =
    | Viewing
    | Creating
    | Updating(Store.item)
    | Deleting(Store.item);

  type state = {
    items: list(Store.item),
    intent,
  };

  type action =
    | UpdateIntent(intent)
    | LoadItems
    | ItemsLoaded(list(Store.item))
    | DeleteItem(Store.item)
    | ItemDeleted(Store.item)
    | UpdateItem(Store.item)
    | ItemUpdated(Store.item)
    | CreateItem(Store.newItem)
    | ItemCreated(Store.item);

  let component = ReasonReact.reducerComponent("ItemManager");

  let goBack = _ => ReasonReact.Router.push("/admin");

  let make =
      (
        ~name,
        ~header,
        ~tableHeaders=[||],
        ~renderCreate,
        ~renderEdit,
        ~renderItem,
        _children,
      ) => {
    ...component,
    initialState: () => {items: [], intent: Viewing},
    didMount: self => self.send(LoadItems),
    reducer: (action, state) =>
      switch (action) {
      | UpdateIntent(intent) => ReasonReact.Update({...state, intent})
      | LoadItems =>
        ReasonReact.SideEffects(
          (
            self =>
              Js.Promise.(
                Store.getAll()
                |> then_(items => resolve(self.send(ItemsLoaded(items))))
                |> ignore
              )
          ),
        )
      | ItemsLoaded(items) => ReasonReact.Update({...state, items})
      | DeleteItem(item) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, intent: Viewing},
          (
            self =>
              Js.Promise.(
                Store.remove(~id=Store.id(item))
                |> then_(() => resolve(self.send(ItemDeleted(item))))
                |> ignore
              )
          ),
        )
      | ItemDeleted(item) =>
        ReasonReact.Update({
          ...state,
          items:
            state.items |> List.filter(i => Store.id(i) !== Store.id(item)),
        })
      | UpdateItem(item) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, intent: Viewing},
          (
            self =>
              Js.Promise.(
                Store.update(item)
                |> then_(item => resolve(self.send(ItemUpdated(item))))
                |> catch(err => resolve(Js.log(err)))
                |> ignore
              )
          ),
        )
      | ItemUpdated(item) =>
        ReasonReact.Update({
          ...state,
          items:
            state.items
            |> List.map(i => Store.id(i) === Store.id(item) ? item : i),
        })
      | CreateItem(item) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, intent: Viewing},
          (
            self =>
              Js.Promise.(
                Store.add(item)
                |> then_(item => resolve(self.send(ItemCreated(item))))
                |> ignore
              )
          ),
        )
      | ItemCreated(item) =>
        ReasonReact.Update({...state, items: state.items @ [item]})
      },
    render: self => {
      let name = String.lowercase(name);
      let capitalizedName = String.capitalize(name);
      <div className="admin-menu">
        <div className="header">
          <div className="header-menu">
            <div
              className="card wide-card pay-button-card"
              onClick=(_ => self.send(UpdateIntent(Creating)))>
              (ReactUtils.s("Create"))
            </div>
            <div className="card wide-card quiet-card" onClick=goBack>
              (ReactUtils.s("Atras"))
            </div>
          </div>
          <div className="header-options"> (ReactUtils.sloc(header)) </div>
        </div>
        <div className={j|$name-management|j}>
          <table className="table">
            <thead>
              <tr>
                <th />
                (
                  tableHeaders
                  |> Array.map(h =>
                       <th key=h> (ReactUtils.sloc({j|$name.$h|j})) </th>
                     )
                  |> ReasonReact.array
                )
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
                         _ => self.send(UpdateIntent(Updating(i))),
                       ~onDeleteClick=
                         _ => self.send(UpdateIntent(Deleting(i))),
                     )
                   )
                |> Array.of_list
                |> ReasonReact.array
              )
            </tbody>
          </table>
        </div>
        (
          switch (self.state.intent) {
          | Viewing => ReasonReact.null
          | Creating =>
            <ItemModal
              isOpen=true
              label={j|action.create$capitalizedName|j}
              onClose=(_ => self.send(UpdateIntent(Viewing)))
              render=(
                () =>
                  renderCreate(
                    ~items=self.state.items,
                    ~onSubmit=item => self.send(CreateItem(item)),
                    ~onCancel=_ => self.send(UpdateIntent(Viewing)),
                  )
              )
            />
          | Updating(item) =>
            <ItemModal
              isOpen=true
              label={j|action.edit$capitalizedName|j}
              onClose=(_ => self.send(UpdateIntent(Viewing)))
              render=(
                () =>
                  renderEdit(
                    ~items=self.state.items,
                    ~item,
                    ~onSubmit=item => self.send(UpdateItem(item)),
                    ~onCancel=_ => self.send(UpdateIntent(Viewing)),
                  )
              )
            />
          | Deleting(item) =>
            <DeleteModal
              isOpen=true
              contentLabel=("modal.delete" ++ capitalizedName ++ "Content")
              label={j|modal.delete$capitalizedName|j}
              onConfirm=(() => self.send(DeleteItem(item)))
              onCancel=(() => self.send(UpdateIntent(Viewing)))
            />
          }
        )
      </div>;
    },
  };
};