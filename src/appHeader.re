let component = ReasonReact.statelessComponent("AppHeader");

let make = _children => {
  ...component,
  render: _self =>
    <div className="App-header">
      <div className="Title"> (ReasonReact.stringToElement("Cafe")) </div>
    </div>,
};