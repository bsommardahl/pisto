let component = ReasonReact.statelessComponent("DisplayOrderItemNotes");
let str = ReasonReact.string;
let make = (~notes: list(OrderItemNote.t), _children) => {
  ...component,
  render: _self =>
    <tr className="note-row">
      <td />
      <td />
      <td colSpan=2>
        <ul>
          (
            notes
            |> List.map((note: OrderItemNote.t) =>
                 <li key=note.id> (str(note.value)) </li>
               )
            |> Array.of_list
            |> ReasonReact.array
          )
        </ul>
      </td>
    </tr>,
};