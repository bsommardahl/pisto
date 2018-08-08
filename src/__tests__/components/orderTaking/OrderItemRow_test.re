open Jest;
open Expect;

Enzyme.configureEnzyme(Enzyme.react_16_adapter());

describe("OrderItemRow", () => {
  let orderItem: OrderItem.t = {
    id: "",
    quantity: 1,
    notes: [],
    sku: "PizzaPep",
    name: "Pizza Peperoni",
    suggestedPrice: 20000,
    taxCalculation: TotalFirst(15),
    addedOn: 1.1,
    salePrice: 20000,
  };

  describe("when order item has values", () => {
    test("it renders the order item's name", () => {
      let wrapper =
        Enzyme.shallow(
          <OrderItemRow
            orderItem
            discounts=[]
            removeOrderItem=(() => ())
            showDialog=(() => ())
            changeQuantity=(_ => ())
          />,
        );
      let td = wrapper |> Enzyme.find("td") |> Enzyme.at(3);
      let name = td |> Enzyme.text;
      expect(name) |> toContainString(orderItem.name);
    });
    test("it renders the order item's subtotal", () => {
      let wrapper =
        Enzyme.shallow(
          <OrderItemRow
            orderItem
            discounts=[]
            removeOrderItem=(() => ())
            showDialog=(() => ())
            changeQuantity=(_ => ())
          />,
        );
      let td = wrapper |> Enzyme.find("td") |> Enzyme.last;
      let subtotal = td |> Enzyme.text;
      expect(subtotal) |> toContainString("173.91");
    });
    test(
      "it renders the order item's suggested price with a fixed precision of 2",
      () => {
      let wrapper =
        Enzyme.shallow(
          <OrderItemRow
            orderItem
            discounts=[]
            removeOrderItem=(() => ())
            showDialog=(() => ())
            changeQuantity=(_ => ())
          />,
        );
      let td = wrapper |> Enzyme.find("td") |> Enzyme.at(4);
      let suggestedPrice = td |> Enzyme.text;
      expect(suggestedPrice) |> toContainString("200.00");
    });
  });
  test("it renders the right number of buttons", () => {
    let wrapper =
      Enzyme.shallow(
        <OrderItemRow
          orderItem
          discounts=[]
          removeOrderItem=(() => ())
          showDialog=(() => ())
          changeQuantity=(_ => ())
        />,
      );
    let buttons = wrapper |> Enzyme.find("Button");
    expect(Enzyme.length(buttons)) |> toBe(2);
  });
});