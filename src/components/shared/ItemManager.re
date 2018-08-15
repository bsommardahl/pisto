module Create = (ItemStore: DbStore.T) => {
  type columnRenderer = {
    nameKey: string,
    render: ItemStore.item => ReasonReact.reactElement,
  };

  type intent =
    | Viewing
    | Creating
    | Updating(ItemStore.item)
    | Deleting(ItemStore.item);

  type state = {
    items: list(ItemStore.item),
    intent,
  };

  type action =
    | UpdateIntent(intent)
    | LoadItems
    | ItemsLoaded(list(ItemStore.item))
    | DeleteItem(ItemStore.item)
    | ItemDeleted(ItemStore.item)
    | UpdateItem(ItemStore.item)
    | ItemUpdated(ItemStore.item)
    | CreateItem(ItemStore.newItem)
    | ItemCreated(ItemStore.item);

  let component = ReasonReact.reducerComponent("ItemManager");

  let goBack = _ => ReasonReact.Router.push("/admin");

  let make =
      (
        ~headerKey,
        ~name,
        ~renderCreate,
        ~renderEdit,
        ~renderColumns,
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
                ItemStore.getAll()
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
                ItemStore.remove(~id=ItemStore.id(item))
                |> then_(() => resolve(self.send(ItemDeleted(item))))
                |> ignore
              )
          ),
        )
      | ItemDeleted(item) =>
        ReasonReact.Update({
          ...state,
          items:
            state.items
            |> List.filter(i => ItemStore.id(i) !== ItemStore.id(item)),
        })
      | UpdateItem(item) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, intent: Viewing},
          (
            self =>
              Js.Promise.(
                ItemStore.update(item)
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
            |> List.map(i =>
                 ItemStore.id(i) === ItemStore.id(item) ? item : i
               ),
        })
      | CreateItem(item) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, intent: Viewing},
          (
            self =>
              Js.Promise.(
                ItemStore.add(item)
                |> then_(item => resolve(self.send(ItemCreated(item))))
                |> ignore
              )
          ),
        )
      | ItemCreated(item) =>
        ReasonReact.Update({...state, items: state.items @ [item]})
      },
    render: self => {
      let lowerCaseName = String.lowercase(name);
      let capitalizedName = String.capitalize(lowerCaseName);
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
          <div className="header-options"> (ReactUtils.sloc(headerKey)) </div>
        </div>
        <div className={j|$lowerCaseName-management|j}>
          <table className="table">
            <thead>
              <tr>
                <th />
                (
                  renderColumns
                  |> Array.mapi((i, {nameKey}) =>
                       <th key=(i |> string_of_int)>
                         (ReactUtils.sloc(nameKey))
                       </th>
                     )
                  |> ReasonReact.array
                )
                <th />
              </tr>
            </thead>
            <tbody>
              (
                self.state.items
                |> List.map(item =>
                     <tr key=(ItemStore.id(item))>
                       <td>
                         <Button
                           local=true
                           disabled=false
                           onClick=(
                             _ => self.send(UpdateIntent(Updating(item)))
                           )
                           label="action.edit"
                         />
                       </td>
                       (
                         renderColumns
                         |> Array.mapi((i, columnRenderer) =>
                              <td key=(i |> string_of_int)>
                                (columnRenderer.render(item))
                              </td>
                            )
                         |> ReasonReact.array
                       )
                       <td>
                         <Button
                           local=true
                           className="danger-card"
                           onClick=(
                             _ => self.send(UpdateIntent(Deleting(item)))
                           )
                           label="action.delete"
                         />
                       </td>
                     </tr>
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