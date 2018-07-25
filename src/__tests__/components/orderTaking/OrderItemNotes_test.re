open Jest;
open Expect;

Enzyme.configureEnzyme(Enzyme.react_16_adapter());

describe("OrderItemsNotes", () =>
  test("it renders the correct amount of notes", () => {
    let notes: list(OrderItemNote.t) = [
      {id: "1", value: "Test note 1"},
      {id: "2", value: "Test note 2"},
      {id: "3", value: "Test note 3"},
    ];
    let wrapper =
      Enzyme.shallow(
        <OrderItemsNotes label="Order Item Notes" notes addNote=(_ => ()) />,
      );
    let trNodes = wrapper |> Enzyme.find("tr");
    expect(Enzyme.length(trNodes)) |> toBe(notes |> List.length);
  })
);