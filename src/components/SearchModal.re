type state = {products: list(Product.t)};

type action =
  | FilterProduct(string);

let component = ReasonReact.reducerComponent("SearchModal");

let lowercase = text => Js.String.toLowerCase(text);

let make =
    (
      ~onCancel=() => (),
      ~onSelect,
      ~isOpen=false,
      ~allProducts: list(Product.t),
      ~label: string,
      _children,
    ) => {
  ...component,
  initialState: () => {products: []},
  reducer: (action, _state) =>
    switch (action) {
    | FilterProduct(text) =>
      ReasonReact.Update({
        products:
          allProducts
          |> List.filter((x: Product.t) =>
               lowercase(x.name)
               |> Js.String.indexOf(lowercase(text)) > (-1)
             ),
      })
    },
  render: self =>
    <div>
      <BsReactstrap.Modal isOpen className="modal">
        <BsReactstrap.ModalHeader className="modal-header">
          (ReactUtils.sloc(label))
          <Button
            className="exit-modal-button-card exit-card"
            onClick=(_ => onCancel())
            iconClass="fas fa-times"
            local=true
          />
        </BsReactstrap.ModalHeader>
        <BsReactstrap.ModalBody className="modal-content">
          <NormalInput onFinish=(text => self.send(FilterProduct(text))) />
          <div className="order-items">
            <table>
              <tbody>
                (
                  self.state.products
                  |> List.map((i: Product.t) =>
                       <tr>
                         <td>
                           <Button
                             onClick=(_ => onSelect(i))
                             label="action.select"
                             className="small-card pay-button-card"
                             local=true
                           />
                         </td>
                         <td> (ReactUtils.s(i.sku)) </td>
                         <td> (ReactUtils.s(i.name)) </td>
                         <td>
                           (ReactUtils.s(i.suggestedPrice |> Money.toDisplay))
                         </td>
                       </tr>
                     )
                  |> Array.of_list
                  |> ReasonReact.arrayToElement
                )
              </tbody>
            </table>
          </div>
        </BsReactstrap.ModalBody>
        <BsReactstrap.ModalFooter className="modal-footer">
          <Button
            local=true
            className="cancel-button-card"
            label="action.cancelModal"
            onClick=(_ => onCancel())
          />
        </BsReactstrap.ModalFooter>
      </BsReactstrap.Modal>
    </div>,
};