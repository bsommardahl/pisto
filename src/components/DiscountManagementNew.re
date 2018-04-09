open Util;

type state = {
  name: string,
  percent: string,
};

type action =
  | ChangeName(string)
  | ChangePercent(string)
  | ClearInputs;

let component = ReasonReact.reducerComponent("DiscountManagementNew");

let make = (~create, _children) => {
  ...component,
  initialState: () => {name: "", percent: ""},
  reducer: (action, state) =>
    switch (action) {
    | ChangeName(newVal) => ReasonReact.Update({...state, name: newVal})
    | ChangePercent(newVal) =>
      ReasonReact.Update({...state, percent: newVal})
    | ClearInputs => ReasonReact.Update({name: "", percent: ""})
    },
  render: self => {
    let finishedEnteringData = () => {
      let newDiscount: Discount.NewDiscount.t = {
        name: self.state.name,
        percent: self.state.percent |> int_of_string,
      };
      self.send(ClearInputs);
      create(newDiscount);
    };
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    <tr>
      <td />
      <td>
        <input
          value=self.state.name
          onChange=(ev => self.send(ChangeName(getVal(ev))))
        />
      </td>
      <td>
        <input
          value=self.state.percent
          onChange=(ev => self.send(ChangePercent(getVal(ev))))
        />
      </td>
      <td>
        <button onClick=((_) => finishedEnteringData())>
          (s("Crear"))
        </button>
      </td>
    </tr>;
  },
};