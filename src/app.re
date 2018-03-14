[%bs.raw {|require('./app.css')|}];

let component = ReasonReact.statelessComponent("App");


let make = ( _children) => {
  ...component,
  render: (_self) =>
    <div className="App">
      <div className="App-header">
        <div className="Title"> (ReasonReact.stringToElement("Cafe")) </div>
      </div>
      <div className="App-intro">
        <StartNewOrder />
      </div>
    </div>
};
