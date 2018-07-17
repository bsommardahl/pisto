let component = ReasonReact.statelessComponent("DisplayOrderItemNotes");
let str = ReasonReact.stringToElement;
let make = (~notes: list(OrderItemNote.t), _children) => {
  ...component,
  render: _self =>
    <div>
      (
        notes
        |> List.map((note: OrderItemNote.t) =>
             <div key=note.id> (str(note.value)) </div>
           )
        |> Array.of_list
        |> ReasonReact.arrayToElement
      )
    </div>,
};