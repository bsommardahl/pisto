let component = ReasonReact.statelessComponent("StartNewOrder");

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