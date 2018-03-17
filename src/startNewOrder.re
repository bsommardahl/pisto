let component = ReasonReact.statelessComponent("StartNewOrder");

/* Here, the button should push to /NewOrder?customerName=john and the view changes */
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
      />
      <button> (ReasonReact.stringToElement("Empezar Orden")) </button>
    </div>,
};