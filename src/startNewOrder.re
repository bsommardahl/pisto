let component = ReasonReact.statelessComponent("StartNewOrder");

let onNameChange: event => (
  ReasonReact.Update({customerName: event.target.value})
);

let make = _children => {
  ...component,
  render: _self =>
    <div id="newOrder">
      <input
        _type="text"
        name="customerName"
        id="customerName"
        placeholder="Nombre del Cliente"
        required=Js.true_
        onChange=(onNameChange)
      />
      /* Here, the button should push to /NewOrder?customerName=john and the view changes */
      <Link href=""> (ReasonReact.stringToElement("Empezar Orden")) </Link>
    </div>,
};